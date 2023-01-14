#include "ClientUtility.h"
#include "UCamera.h"

using namespace client;

ClientUtility& ClientUtility::getInstance()
{
		static ClientUtility m_instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return m_instance;
}

std::shared_ptr<utopia::UNode> ClientUtility::findGameObjectByName(std::shared_ptr<utopia::UNode> root, const std::string& name) const
{
	for (auto& node : root->getChildren())
	{
		if (node->getName() == name)
			return node;
		else
		{
			auto childByName = ClientUtility::findGameObjectByName(node, name);
			if (childByName != nullptr)
			{
				return childByName;
			}
			else
				continue;
		}
	}
	return nullptr;
}

std::vector<std::shared_ptr<utopia::UNode>> client::ClientUtility::findGameObjectsByName(std::shared_ptr<utopia::UNode> root, const std::string& name) const
{
	std::vector< std::shared_ptr<utopia::UNode>> vector;
	for (auto& node : root->getChildren())
	{
		if (node->getName() == name)
			vector.push_back(node);
		else
		{
			auto childByName = ClientUtility::findGameObjectByName(node, name);
			if (childByName != nullptr)
			{
				findGameObjectsByNameIntermediate(vector,node,name);
			}
			else
				continue;
		}
	}
	return vector;
}

glm::vec3 client::ClientUtility::getLocalPosition(std::shared_ptr<utopia::UNode> node) const
{
	auto& modelView = node->getModelView();
	return glm::vec3(modelView[3][0], modelView[3][1], modelView[3][2]);
}

glm::vec3 client::ClientUtility::getWorldPosition(std::shared_ptr<utopia::UNode> node) const
{
	auto modelView = node->getFinalWorldCoordinates();
	return glm::vec3(modelView[3][0], modelView[3][1], modelView[3][2]);
}

void ClientUtility::findGameObjectsByNameIntermediate(std::vector<std::shared_ptr<utopia::UNode>> vector, std::shared_ptr<utopia::UNode> root, const std::string& name) const
{
	for (auto& node : root->getChildren())
	{
		if (node->getName() == name)
			vector.push_back(node);
		else
		{
			auto childByName = ClientUtility::findGameObjectByName(node, name);
			if (childByName != nullptr)
			{
				findGameObjectsByNameIntermediate(vector, node, name);
			}
			else
				continue;
		}
	}
}


ClientUtility::ClientUtility() {};
ClientUtility::~ClientUtility() {};
