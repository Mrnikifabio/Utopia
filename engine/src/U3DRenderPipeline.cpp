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
		auto renderNode = std::unique_ptr<U3DRenderNode>(new U3DRenderNode(node, mat));
		m_pimpl->m_lights.push_back(std::move(renderNode));
	}
	else if (dynamic_cast<UMesh*>(node.lock().get()))
	{
		auto renderNode = std::unique_ptr<U3DRenderNode>(new U3DRenderNode(node, mat, material));
		m_pimpl->m_nodes.push_back(std::move(renderNode));
	}
	else
	{
		auto renderNode = std::unique_ptr<U3DRenderNode>(new U3DRenderNode(node, mat));
		m_pimpl->m_nodes.push_back(std::move(renderNode));
	}
}

void U3DRenderPipeline::render()
{
	for (auto& node : m_pimpl->m_nodes)
	{
		node->node.lock()->setModelView(node->mat);
		node->node.lock()->render();
	}
	for (auto& node : m_pimpl->m_lights)
	{
		node->node.lock()->setModelView(node->mat);
		if (dynamic_cast<UMesh*>(node->node.lock().get()))
		{
			if (node->material != nullptr)
				((UMesh*)node->node.lock().get())->setMaterial(node->material);
		}
		node->node.lock()->render();
	}
}
