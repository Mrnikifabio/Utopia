#include "Tower.h"
#include "ClientUtility.h"
#include <glm/ext/matrix_transform.hpp>
#include <memory>

using namespace client;

struct Tower::pimpl
{
	std::shared_ptr<utopia::UNode> m_tower;
	std::shared_ptr<utopia::UNode> m_hook;
	std::shared_ptr<utopia::UNode> m_Fisicalhook;

	float m_hookLimitForward=0;
	float m_hookLimitBackward=0;

	float m_hookLimitUp=0;
	float m_hookLimitDown=0;

	pimpl(std::shared_ptr<utopia::UNode> tower)
	{
		m_tower = tower;
	}

	pimpl()
	{
	}
};

Tower::Tower(std::shared_ptr<utopia::UNode> tower) : m_pimpl{ std::make_unique<Tower::pimpl>(tower) } {};
Tower::Tower() : m_pimpl{ std::make_unique<Tower::pimpl>() } {}
client::Tower::~Tower() {};

void Tower::setHook(std::shared_ptr<utopia::UNode> hook)
{
	m_pimpl->m_hook = hook;
}

void client::Tower::setFisicalHook(std::shared_ptr<utopia::UNode> fisicalHook)
{
	m_pimpl->m_Fisicalhook = fisicalHook;
}

void Tower::setTower(std::shared_ptr<utopia::UNode> tower)
{
	m_pimpl->m_tower = tower;
}

void Tower::setHookLimitforward(float value)
{
	m_pimpl->m_hookLimitForward = value;
}

void Tower::setHookLimitbackward(float value)
{
	m_pimpl->m_hookLimitBackward = value;

}


void Tower::setFisicalHookLimitUp(float value)
{
	m_pimpl->m_hookLimitUp = value;
}

void Tower::setFisicalHookLimitDown(float value)
{
	m_pimpl->m_hookLimitDown = value;
}

void Tower::rotateTower(glm::f32 angle)
{
	m_pimpl->m_tower->setModelView(glm::rotate(m_pimpl->m_tower->getModelView(), angle, glm::vec3(0.f, 1.f, 0.f)));
}

void Tower::moveFisicalHookUpDown(float distance)
{
	float currentYPosition = client::ClientUtility::getInstance().getLocalPosition(m_pimpl->m_Fisicalhook).y;
	if (currentYPosition + distance < m_pimpl->m_hookLimitDown|| currentYPosition + distance > m_pimpl->m_hookLimitUp)
	{
		return;
	}

	m_pimpl->m_Fisicalhook->setModelView(glm::translate(m_pimpl->m_Fisicalhook->getModelView(), glm::vec3(0.f, distance, 0.f)));

}

void client::Tower::moveHookBackwardForward(float distance)
{
	float currentXPosition = client::ClientUtility::getInstance().getLocalPosition(m_pimpl->m_hook).x;
	if (currentXPosition + distance < m_pimpl->m_hookLimitBackward|| currentXPosition + distance > m_pimpl->m_hookLimitForward)
	{
		return;
	}

	m_pimpl->m_hook->setModelView(glm::translate(m_pimpl->m_hook->getModelView(), glm::vec3(distance, 0.f, 0.f)));
}


