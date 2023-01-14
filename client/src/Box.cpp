#include "Box.h"
#include <glm/ext/matrix_transform.hpp>
#include "ClientUtility.h"
#include "UCamera.h"

using namespace client;

struct Box::pimpl
{
	std::shared_ptr<utopia::UNode> m_hookPoint=nullptr;
	std::shared_ptr<utopia::UNode> m_groundNode = nullptr;
	std::shared_ptr<utopia::UNode> m_topNode = nullptr;

	bool m_isHooked=false;
	bool m_isGrounded=false;
	
};

client::Box::Box() : m_pimpl{ std::unique_ptr<pimpl>(new pimpl()) } {};

client::Box::~Box()
{
}

void client::Box::setHookPointNode(std::shared_ptr<utopia::UNode> hookPointNode)
{
	m_pimpl->m_hookPoint = hookPointNode;
}

void client::Box::setGroundNode(std::shared_ptr<utopia::UNode> groundNode)
{
	m_pimpl->m_groundNode = groundNode;

}

void client::Box::setTopNode(std::shared_ptr<utopia::UNode> topNode)
{
	m_pimpl->m_topNode = topNode;
}

std::shared_ptr<utopia::UNode> client::Box::getHookPointNode() const
{
	return m_pimpl->m_hookPoint;
}

std::shared_ptr<utopia::UNode> client::Box::getGroundNode() const
{
	return m_pimpl->m_groundNode;
}

std::shared_ptr<utopia::UNode> client::Box::getTopNode() const
{
	return m_pimpl->m_topNode;
}

void client::Box::computeGravity()
{
	if (!m_pimpl->m_isGrounded||!m_pimpl->m_isHooked)
	{
		m_pimpl->m_hookPoint->setModelView(glm::translate(m_pimpl->m_hookPoint->getModelView(), glm::vec3(0.f,- 2.0f, 0.f)));
	
		auto boxPosition = client::ClientUtility::getInstance().getWorldPosition(m_pimpl->m_groundNode);
		
		if (boxPosition.y < 0)
		{
			m_pimpl->m_isGrounded = true;
			m_pimpl->m_hookPoint->setModelView(glm::translate(m_pimpl->m_hookPoint->getModelView(), glm::vec3(0.f, boxPosition.y*-1, 0.f)));
		}
	}
}

bool client::Box::isHooked() const
{
	return m_pimpl->m_isHooked;
}

void client::Box::take()
{
	m_pimpl->m_hookPoint->setModelView(glm::mat4(1));
	m_pimpl->m_isGrounded = true;
	m_pimpl->m_isHooked = true;
}


void client::Box::release()
{
	m_pimpl->m_hookPoint->setModelView(m_pimpl->m_hookPoint->getFinalWorldCoordinates());
	m_pimpl->m_isHooked = false;
}
