#include "U3DRenderPipeline.h"
#include "UCamera.h"
#include "ULight.h"
#include "UMesh.h"
#include "UMaterial.h"
#include <vector>

using namespace utopia;

struct U3DRenderPipeline::pimpl {
	std::vector<std::unique_ptr<U3DRenderNode>> m_nodes;
	std::vector<std::unique_ptr<U3DRenderNode>> m_lights;
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
	m_pimpl->m_nodes.clear();
	m_pimpl->m_lights.clear();
}

void U3DRenderPipeline::render()
{
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

	for (auto& node : m_pimpl->m_lights)
	{
		auto oldMat = node->node.lock()->getModelView(); //the matrix is COPIED no auto&
		node->node.lock()->setModelView(node->mat);

		node->node.lock()->render();

		node->node.lock()->setModelView(oldMat);
	}
}
