#include "U3DRenderPipeline.h"
#include "UCamera.h"
#include <vector>

using namespace utopia;

struct U3DRenderPipeline::pimpl {
	std::vector<std::unique_ptr<U3DRenderNode>> m_nodes;
};

struct U3DRenderPipeline::U3DRenderNode {
	std::weak_ptr<UNode> node;
	glm::mat4 mat;

	U3DRenderNode(std::weak_ptr<UNode> node, const glm::mat4& mat)
	{
		this->node = node;
		this->mat = mat;
	}
};

U3DRenderPipeline::U3DRenderPipeline(const std::string& name) : UObject(name), m_pimpl{ std::unique_ptr<pimpl>(new pimpl()) } {}

U3DRenderPipeline::~U3DRenderPipeline() = default;

void U3DRenderPipeline::pass(std::weak_ptr<UNode> node, const glm::mat4& mat)
{
	auto uRenderNode = std::unique_ptr<U3DRenderNode>(new U3DRenderNode(node, mat));
	m_pimpl->m_nodes.push_back(std::move(uRenderNode));
}

void U3DRenderPipeline::render()
{
	for (auto& node : m_pimpl->m_nodes)
	{
		node->node.lock()->setModelView(node->mat);
		node->node.lock()->render();
	}
}
