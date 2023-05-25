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
#include "UExtensions.h"
#include <glm/gtx/string_cast.hpp>
#include "ovr.h"
#include "USkybox.h"

using namespace utopia;

struct U3DRenderPipeline::pimpl {
	std::vector<std::unique_ptr<U3DRenderNode>> m_nodes;
	std::vector<std::unique_ptr<U3DRenderNode>> m_lights;
	std::vector<std::unique_ptr<U2DQuad>> m_eyes;
	std::unique_ptr<U2DQuad> m_screen;
	std::unique_ptr<USkybox> m_skybox;
	bool m_first;

	pimpl() : m_first{ true }, m_skybox{ nullptr } {
		
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

void U3DRenderPipeline::enableSkybox(std::shared_ptr<UCubeMapTexture> cubeMap, const glm::mat4& modelView)
{
	if (m_pimpl->m_skybox == nullptr)
	{
		m_pimpl->m_skybox = std::unique_ptr<USkybox>(new USkybox(getName() + "Skybox"));
		m_pimpl->m_skybox->setModelView(modelView);
	}
	m_pimpl->m_skybox->setTexture(cubeMap);
}

void U3DRenderPipeline::disableSkybox()
{
	m_pimpl->m_skybox = nullptr;
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
		if (isStereoscopic) {
			auto uvr = Utopia::getInstance().getOpenVRWrapper();
			//the lines commented are the eyes for a window in the screen
			//m_pimpl->m_eyes.push_back(std::unique_ptr<U2DQuad>(new U2DQuad("left eye", APP_WINDOWSIZEX / 2, APP_WINDOWSIZEY, APP_WINDOWSIZEX, APP_WINDOWSIZEY, glm::mat4(1.0f))));
			//m_pimpl->m_eyes.push_back(std::unique_ptr<U2DQuad>(new U2DQuad("right eye", APP_WINDOWSIZEX / 2, APP_WINDOWSIZEY, APP_WINDOWSIZEX, APP_WINDOWSIZEY, glm::translate(glm::mat4(1.0f), glm::vec3(APP_WINDOWSIZEX / 2.0f, 0.0f, 0.0f)))));
			m_pimpl->m_eyes.push_back(std::unique_ptr<U2DQuad>(new U2DQuad("left eye", uvr->getHmdIdealHorizRes(), uvr->getHmdIdealVertRes(), uvr->getHmdIdealHorizRes()*2, uvr->getHmdIdealVertRes(), glm::mat4(1.0f))));
			m_pimpl->m_eyes.push_back(std::unique_ptr<U2DQuad>(new U2DQuad("right eye", uvr->getHmdIdealHorizRes(), uvr->getHmdIdealVertRes(), uvr->getHmdIdealHorizRes()*2, uvr->getHmdIdealVertRes(), glm::translate(glm::mat4(1.0f), glm::vec3(uvr->getHmdIdealHorizRes(), 0.0f, 0.0f)))));
			for (auto& eye : m_pimpl->m_eyes)
				eye->init();
		}
		else {
			m_pimpl->m_screen = std::unique_ptr<U2DQuad>(new U2DQuad("screen", APP_WINDOWSIZEX, APP_WINDOWSIZEY, APP_WINDOWSIZEX, APP_WINDOWSIZEY, glm::mat4(1.0f)));
			m_pimpl->m_screen->init();
		}
		m_pimpl->m_first = false;
	}

	if (isStereoscopic) {
		// Update user position:
		ovr->update();
		glm::mat4 headPos = ovr->getModelviewMatrix();

		for (int c = 0; c < OvVR::EYE_LAST; c++)
		{
			// Get OpenVR matrices:
			OvVR::OvEye curEye = (OvVR::OvEye)c;
			glm::mat4 projMat = ovr->getProjMatrix(curEye, UCamera::getMainCamera().lock()->getNear(), UCamera::getMainCamera().lock()->getFar());
			glm::mat4 eye2Head = ovr->getEye2HeadMatrix(curEye);

			// Update camera projection matrix:
			glm::mat4 ovrProjMat = projMat * glm::inverse(eye2Head);
			if (dynamic_cast<UOVRCamera*>(UCamera::getMainCamera().lock().get()))
				((UOVRCamera*)UCamera::getMainCamera().lock().get())->setCameraMatrix(ovrProjMat);

			UCamera::getMainCamera().lock()->setModelView(headPos);

			auto& eye = m_pimpl->m_eyes[c];
			eye->activeAsBuffer();
			renderInBuffer();
			eye->disableAsBuffer();
			
			ovr->pass(curEye, eye->getTextureID());
		}
	}
	else {
		m_pimpl->m_screen->activeAsBuffer();
		renderInBuffer();
		m_pimpl->m_screen->disableAsBuffer();
	}

	glViewport(0, 0, prevViewport[2], prevViewport[3]);

	if (isStereoscopic) {
		//render in vr device (pass texture)
		ovr->render();
		//render in screen
		for (auto& eye : m_pimpl->m_eyes)
			eye->render();
	}
	else {
		m_pimpl->m_screen->render();
	}
}

void U3DRenderPipeline::renderInBuffer() 
{
	auto camera = UCamera::getMainCamera().lock();
	auto centerDistance = (camera->getNear() + camera->getFar()) / 2; // middle point between near and far plane
	auto centerSphere= glm::vec3(0,0, -centerDistance);  // center of the sphere in camera space
	auto radiousSphere = centerDistance + camera->getNear();


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

				
				// check if the node is inside the sphere
				auto matrixEyeCoordinates= glm::inverse(camera->getFinalWorldCoordinates()) * node->node.lock()->getModelView();
				auto distance = glm::distance(centerSphere, glm::getPositionFromMatrix(matrixEyeCoordinates));
				
				
				if (distance < radiousSphere + ((UMesh*)node->node.lock().get())->getRadious())
				{
					node->node.lock()->render();
				}
			}
			else
				node->node.lock()->render(); //render the node if it is not a mesh

			node->node.lock()->setModelView(oldMat);
			if (dynamic_cast<UMesh*>(node->node.lock().get()))
			{
				((UMesh*)node->node.lock().get())->setMaterial(oldMaterial);
			}
		}
	}

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);


	//if a skybox has been requested
	if (m_pimpl->m_skybox != nullptr)
		m_pimpl->m_skybox->render();
}
