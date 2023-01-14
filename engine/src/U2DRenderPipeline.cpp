#include "Utopia.h"
#include "U2DRenderPipeline.h"
#include "UOrthographicCamera.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace utopia;

struct U2DRenderPipeline::pimpl {
	std::vector<std::unique_ptr<U2DRenderNode>> m_nodes;
};

struct U2DRenderPipeline::U2DRenderNode {
	std::weak_ptr<U2DObject> m_node;
	glm::vec2 m_pos;

	U2DRenderNode(std::weak_ptr<U2DObject> node, const glm::vec2& pos) : m_node{ node }, m_pos{ pos } {}
};


U2DRenderPipeline::U2DRenderPipeline(const std::string& name) : UObject(name), m_pimpl{ std::unique_ptr<pimpl>(new pimpl()) } {}

U2DRenderPipeline::~U2DRenderPipeline() = default;

void U2DRenderPipeline::clear()
{
	m_pimpl->m_nodes.clear();
}

void U2DRenderPipeline::pass(std::weak_ptr<U2DObject> node, const glm::vec2& pos)
{
	auto renderNode = std::unique_ptr<U2DRenderNode>(new U2DRenderNode(node, pos));
	m_pimpl->m_nodes.push_back(std::move(renderNode));
}

void U2DRenderPipeline::render()
{
	// Switch to 2D rendering:
	// Switch to window coordinates to render
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, (float)Utopia::getInstance().getWindowWidth(), 0, (float)Utopia::getInstance().getWindowHeight());

	// Disable lighting before rendering 2D text:
	glDisable(GL_LIGHTING);

	for (auto& node : m_pimpl->m_nodes)
	{
		auto tempPos = node->m_node.lock()->getPosition();
		node->m_node.lock()->setPosition(node->m_pos);
		node->m_node.lock()->render();
		node->m_node.lock()->setPosition(tempPos);
	}

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
