#include "URenderPipeline.h"
#include "UCamera.h"
#include <vector>

using namespace utopia;

struct URenderPipeline::pimpl {
	std::vector<std::unique_ptr<URenderNode>> m_nodes;
};

struct URenderPipeline::URenderNode {
	std::weak_ptr<UNode> node;
	glm::mat4 mat;

	URenderNode(std::weak_ptr<UNode> node, const glm::mat4& mat)
	{
		this->node = node;
		this->mat = mat;
	}
};

URenderPipeline::URenderPipeline(const std::string& name) : m_pimpl{ std::make_unique<pimpl>() }, UObject(name) {}

URenderPipeline::~URenderPipeline() = default;

void URenderPipeline::pass(std::weak_ptr<UNode> node, const glm::mat4& mat)
{
	auto uRenderNode = std::make_unique<URenderNode>(node, mat);
	m_pimpl->m_nodes.push_back(std::move(uRenderNode));
}

void URenderPipeline::render()
{
	for (auto& node : m_pimpl->m_nodes)
	{
		node->node.lock()->setModelView(node->mat);
		node->node.lock()->render();
	}
}
