#include "ClientUtility.h"
#include "BoxesManager.h"
#include "BoxesManager.h"
#include "BoxesManager.h"


using namespace client;

struct BoxesManager::pimpl
{
	std::vector<std::shared_ptr<Box>> m_boxes;
};


client::BoxesManager::BoxesManager() : m_pimpl{ std::unique_ptr<pimpl>(new pimpl()) } {};

client::BoxesManager::~BoxesManager()
{

};

std::shared_ptr<Box> client::BoxesManager::possibleBoxToHook(std::shared_ptr<utopia::UNode> hook, float distance) const
{
	glm::vec3 hookPosition = ClientUtility::getInstance().getWorldPosition(hook);
	for (auto& box : m_pimpl->m_boxes)
	{
		float distanceBetweenObject = glm::distance(hookPosition, ClientUtility::getInstance().getWorldPosition(box->getHookPointNode()));
		if (distanceBetweenObject <= distance)
			return box;
	}
	return nullptr;
}

void client::BoxesManager::setBoxes(std::vector<std::shared_ptr<Box>>& boxes)
{
	m_pimpl->m_boxes = boxes;
}

std::vector<std::shared_ptr<Box>> client::BoxesManager::getBoxes() const
{
	return m_pimpl->m_boxes;
}


void client::BoxesManager::computeGravity()
{
	for (auto& box : m_pimpl->m_boxes)
	{
		box->computeGravity();
	}
}

