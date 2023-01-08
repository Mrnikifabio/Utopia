#include "ClientUtility.h"

using namespace client;

ClientUtility& ClientUtility::getInstance()
{
		static ClientUtility m_instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return m_instance;
}

std::shared_ptr<utopia::UNode> ClientUtility::findGameObjectByName(std::shared_ptr<utopia::UNode> root, const std::string& name)
{
	for (auto node : root->getChildren())
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



ClientUtility::ClientUtility() {};
ClientUtility::~ClientUtility() {};
