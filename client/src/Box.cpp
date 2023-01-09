#include "Box.h"
#include <glm/ext/matrix_transform.hpp>
#include "ClientUtility.h"
#include "UCamera.h"

using namespace client;

struct Box::pimpl
{
	std::shared_ptr<utopia::UNode> m_hookPoint;
	std::shared_ptr<utopia::UNode> m_groundNode;
	std::shared_ptr<utopia::UNode> m_topNode;

	bool m_isHooked;
	bool m_isGrounded;
	
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

std::shared_ptr<utopia::UNode> client::Box::getHookPointNode()
{
	return m_pimpl->m_hookPoint;
}

std::shared_ptr<utopia::UNode> client::Box::getGroundNode()
{
	return m_pimpl->m_groundNode;
}

std::shared_ptr<utopia::UNode> client::Box::getTopNode()
{
	return m_pimpl->m_topNode;
}

void client::Box::computeGravity()
{
	if (!m_pimpl->m_isGrounded||!m_pimpl->m_isHooked)
	{
		m_pimpl->m_hookPoint->setModelView(glm::translate(m_pimpl->m_hookPoint->getModelView(), glm::vec3(0.f,- 0.3f, 0.f)));
	
		auto boxPosition = client::ClientUtility::getInstance().getWorldPosition(m_pimpl->m_groundNode);
		
		if (boxPosition.y < 0)
		{
			m_pimpl->m_isGrounded = true;
			m_pimpl->m_hookPoint->setModelView(glm::translate(m_pimpl->m_hookPoint->getModelView(), glm::vec3(0.f, boxPosition.y*-1, 0.f)));
		}
	}
}

bool client::Box::isHooked()
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
	m_pimpl->m_hookPoint->setModelView(utopia::UCamera::getMainCamera().lock()->getModelView()*m_pimpl->m_hookPoint->getFinalWorldCoordinates());
	m_pimpl->m_isHooked = false;
}