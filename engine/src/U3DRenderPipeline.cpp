#include "U3DRenderPipeline.h"
#include "UCamera.h"
#include "ULight.h"
#include <GL/glew.h>
#include <gl/freeglut.h>
#include "UMesh.h"
#include "UMaterial.h"
#include <vector>
#include <U2DQuad.h>
#include <Utopia.h>
#include "UOVRCamera.h"
#include "uvr.h"

using namespace utopia;

struct U3DRenderPipeline::pimpl {
	std::vector<std::unique_ptr<U3DRenderNode>> m_nodes;
	std::vector<std::unique_ptr<U3DRenderNode>> m_lights;
	std::vector<std::unique_ptr<U2DQuad>> m_eyes;
	std::unique_ptr<U2DQuad> m_screen;
	bool m_first;

	pimpl() : m_first { true } {
		m_eyes.push_back(std::unique_ptr<U2DQuad>(new U2DQuad("left eye", glm::vec2(0.0f, 0.0f), glm::vec2(APP_WINDOWSIZEX / 2.0f, APP_WINDOWSIZEY), APP_WINDOWSIZEX, APP_WINDOWSIZEY, glm::mat4(1.0f))));
		m_eyes.push_back(std::unique_ptr<U2DQuad>(new U2DQuad("right eye", glm::vec2(0.0f, 0.0f), glm::vec2(APP_WINDOWSIZEX / 2.0f, APP_WINDOWSIZEY), APP_WINDOWSIZEX, APP_WINDOWSIZEY, glm::translate(glm::mat4(1.0f), glm::vec3(APP_WINDOWSIZEX / 2.0f, 0.0f, 0.0f)))));
		m_screen = std::unique_ptr<U2DQuad>(new U2DQuad("screen", glm::vec2(0.0f, 0.0f), glm::vec2(APP_WINDOWSIZEX, APP_WINDOWSIZEY), APP_WINDOWSIZEX, APP_WINDOWSIZEY, glm::mat4(1.0f)));
	}
};

struct U3DRenderPipeline::U3DRenderNode {
	std::weak_ptr<UNode> node;
	glm::mat4 mat;
	std::shared_ptr<UMaterial> material;

	U3DRenderNode(std::weak_ptr<UNode> node, const glm::mat4& mat)
	{
		this->node = node;
		this->mat = mat;
		material = nullptr;
	}

	U3DRenderNode(std::weak_ptr<UNode> node, const glm::mat4& mat, std::shared_ptr<UMaterial> material)
	{
		this->node = node;
		this->mat = mat;
		this->material = material;
	}
};

U3DRenderPipeline::U3DRenderPipeline(const std::string& name) : UObject(name), m_pimpl{ std::unique_ptr<pimpl>(new pimpl()) } {}

U3DRenderPipeline::~U3DRenderPipeline() = default;

void U3DRenderPipeline::pass(std::weak_ptr<UNode> node, const glm::mat4& mat, std::shared_ptr<UMaterial> material)
{
	if (dynamic_cast<ULight*>(node.lock().get()))
	{
		auto renderNode = std::unique_ptr<U3DRenderNode>(new U3DRenderNode(node, mat * node.lock()->getFinalWorldCoordinates()));
		m_pimpl->m_lights.push_back(std::move(renderNode));
	}
	else if (dynamic_cast<UMesh*>(node.lock().get()))
	{
		auto renderNode = std::unique_ptr<U3DRenderNode>(new U3DRenderNode(node, mat * node.lock()->getFinalWorldCoordinates(), material));
		m_pimpl->m_nodes.push_back(std::move(renderNode));
	}
	else
	{
		auto renderNode = std::unique_ptr<U3DRenderNode>(new U3DRenderNode(node, mat * node.lock()->getFinalWorldCoordinates()));
		m_pimpl->m_nodes.push_back(std::move(renderNode));
	}

	for (auto& child : node.lock()->getChildren())
		pass(child, mat, material);
}

void U3DRenderPipeline::clear()
{
	m_pimpl->m_lights.clear();
	m_pimpl->m_nodes.clear();
}

void U3DRenderPipeline::render()
{
	// Store the current viewport size:
	GLint prevViewport[4];
	glGetIntegerv(GL_VIEWPORT, prevViewport);

	auto ovr = Utopia::getInstance().getOpenVRWrapper();

	bool isStereoscopic = Utopia::getInstance().isStereoscopicEnabled();
	if (m_pimpl->m_first) {
		for (auto& eye : m_pimpl->m_eyes)
			eye->init();
		m_pimpl->m_screen->init();
		m_pimpl->m_first = false;
	}

	if (isStereoscopic) {
		// Update user position:
		ovr->update();
		glm::mat4 headPos = ovr->getModelviewMatrix();

		for (int c = 0; c < UVR::EYE_LAST; c++)
		{
			// Get OpenVR matrices:
			UVR::OvEye curEye = (UVR::OvEye)c;
			glm::mat4 projMat = ovr->getProjMatrix(curEye, 1.0f, 1024.0f);
			glm::mat4 eye2Head = ovr->getEye2HeadMatrix(curEye);

			// Update camera projection matrix:
			glm::mat4 ovrProjMat = projMat * glm::inverse(eye2Head);
			if (dynamic_cast<UOVRCamera*>(UCamera::getMainCamera().lock().get()))
				((UOVRCamera*)UCamera::getMainCamera().lock().get())->setCameraMatrix(ovrProjMat);

			// Update camera modelview matrix:
			glm::mat4 ovrModelViewMat = glm::inverse(headPos); // Inverted because this is the camera matrix
			UCamera::getMainCamera().lock()->setModelView(ovrModelViewMat);

			auto& eye = m_pimpl->m_eyes[c];
			eye->activeAsBuffer();
			renderInBuffer(curEye);
			eye->disableAsBuffer();
			
			ovr->pass(curEye, eye->getTextureID());
		}
	}
	else {
		m_pimpl->m_screen->activeAsBuffer();
		renderInBuffer(UVR::EYE_LAST);
		m_pimpl->m_screen->disableAsBuffer();
	}

	glViewport(0, 0, prevViewport[2], prevViewport[3]);

	if (isStereoscopic) {
		ovr->render();
		for (auto& eye : m_pimpl->m_eyes)
			eye->render();
	}
	else {
		m_pimpl->m_screen->render();
	}
}

void U3DRenderPipeline::renderInBuffer(UVR::OvEye eye) 
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int i = 0;
	for (auto& light : m_pimpl->m_lights)
	{
		if (i == 1)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glDepthFunc(GL_LEQUAL);
		}
		i++;

		auto oldMat = light->node.lock()->getModelView(); //the matrix is COPIED no auto&
		light->node.lock()->setModelView(light->mat);
		light->node.lock()->render();
		light->node.lock()->setModelView(oldMat);


		for (auto& node : m_pimpl->m_nodes)
		{
			auto oldMat = node->node.lock()->getModelView(); //the matrix is COPIED no auto&
			node->node.lock()->setModelView(node->mat);

			std::shared_ptr<UMaterial> oldMaterial = nullptr;
			if (dynamic_cast<UMesh*>(node->node.lock().get()))
			{
				oldMaterial = ((UMesh*)node->node.lock().get())->getMaterial();
				if (node->material != nullptr)
					((UMesh*)node->node.lock().get())->setMaterial(node->material);
			}

			node->node.lock()->render();

			node->node.lock()->setModelView(oldMat);
			if (dynamic_cast<UMesh*>(node->node.lock().get()))
				((UMesh*)node->node.lock().get())->setMaterial(oldMaterial);
		}
	}

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);
}
