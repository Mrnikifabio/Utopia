#include "Tower.h"
#include "ClientUtility.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

using namespace client;

struct Tower::pimpl
{
	std::shared_ptr<utopia::UNode> m_root;
	std::shared_ptr<utopia::UNode> m_tower;
	std::shared_ptr<utopia::UNode> m_hook;
	std::shared_ptr<utopia::UNode> m_Fisicalhook;
	std::shared_ptr<utopia::UNode> m_cable;

	std::shared_ptr<Box> m_hookedBox;


	bool isHooking=false;

	glm::mat4 m_startCableModelView;

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

Tower::Tower(std::shared_ptr<utopia::UNode> tower) : m_pimpl{ std::unique_ptr<Tower::pimpl>(new Tower::pimpl(tower)) } {};
Tower::Tower() : m_pimpl{ std::unique_ptr<Tower::pimpl>(new Tower::pimpl()) } {}
client::Tower::~Tower() {};

void client::Tower::setRoot(std::shared_ptr<utopia::UNode> root)
{
	m_pimpl->m_root = root;
}

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

void client::Tower::setCable(std::shared_ptr<utopia::UNode> cable)
{
	m_pimpl->m_cable = cable;
	m_pimpl->m_startCableModelView = cable->getModelView();
}

std::shared_ptr<utopia::UNode> client::Tower::getRoot() const
{
	return m_pimpl->m_root;
}

std::shared_ptr<utopia::UNode> client::Tower::getHook() const
{
	return m_pimpl->m_hook;
}

std::shared_ptr<utopia::UNode> client::Tower::getFisicalHook() const
{
	return m_pimpl->m_Fisicalhook;
}

std::shared_ptr<utopia::UNode> client::Tower::getTower() const
{
	return m_pimpl->m_tower;
}

std::shared_ptr<utopia::UNode> client::Tower::getCable() const
{
	return m_pimpl->m_cable;
}

bool client::Tower::isHooking() const
{
	return m_pimpl->isHooking;
}

void client::Tower::take(std::shared_ptr<Box> box)
{
	m_pimpl->m_hookedBox = box;
	m_pimpl->m_root->detachChildById(box->getHookPointNode()->getId());
	m_pimpl->m_Fisicalhook->addChild(box->getHookPointNode());
	box->take();
	m_pimpl->isHooking = true;
}

void client::Tower::release()
{
	m_pimpl->m_hookedBox->release();
	m_pimpl->m_Fisicalhook->detachChildById(m_pimpl->m_hookedBox->getHookPointNode()->getId());
	m_pimpl->m_root->addChild(m_pimpl->m_hookedBox->getHookPointNode());
	m_pimpl->isHooking = false;
	m_pimpl->m_hookedBox.reset();
}


std::shared_ptr<utopia::UNode> client::Tower::getFisicalHook()
{
	return m_pimpl->m_Fisicalhook;
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


	float offsetFromStart = - (currentYPosition - (distance * (distance>0?1.f:-1.f) / 2)) / 100;

	if (m_pimpl->m_cable != nullptr)
	{
		m_pimpl->m_cable->setModelView(glm::scale(m_pimpl->m_startCableModelView, glm::vec3(1.f, offsetFromStart, 1.f)));
	}

	std::cout << "offset from start: " << offsetFromStart << std::endl;

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


