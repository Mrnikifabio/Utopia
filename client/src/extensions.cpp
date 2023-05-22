#include "extensions.h"
#include "UCamera.h"

namespace client {

	namespace extensions {

		std::shared_ptr<utopia::UNode> findGameObjectByName(std::shared_ptr<utopia::UNode> root, const std::string& name)
		{
			for (auto& node : root->getChildren())
			{
				if (node->getName() == name)
					return node;
				else
				{
					auto childByName = findGameObjectByName(node, name);
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

		void findGameObjectsByNameIntermediate(std::vector<std::shared_ptr<utopia::UNode>> vector, std::shared_ptr<utopia::UNode> root, const std::string& name)
		{
			for (auto& node : root->getChildren())
			{
				if (node->getName() == name)
					vector.push_back(node);
				else
				{
					auto childByName = findGameObjectByName(node, name);
					if (childByName != nullptr)
					{
						findGameObjectsByNameIntermediate(vector, node, name);
					}
					else
						continue;
				}
			}
		}

		std::vector<std::shared_ptr<utopia::UNode>> findGameObjectsByName(std::shared_ptr<utopia::UNode> root, const std::string& name)
		{
			std::vector< std::shared_ptr<utopia::UNode>> vector;
			for (auto& node : root->getChildren())
			{
				if (node->getName() == name)
					vector.push_back(node);
				else
				{
					auto childByName = findGameObjectByName(node, name);
					if (childByName != nullptr)
					{
						findGameObjectsByNameIntermediate(vector, node, name);
					}
					else
						continue;
				}
			}
			return vector;
		}

		glm::vec3 getLocalPosition(std::shared_ptr<utopia::UNode> node)
		{
			auto& modelView = node->getModelView();
			return glm::vec3(modelView[3][0], modelView[3][1], modelView[3][2]);
		}

		glm::vec3 getWorldPosition(std::shared_ptr<utopia::UNode> node)
		{
			auto modelView = node->getFinalWorldCoordinates();
			return glm::vec3(modelView[3][0], modelView[3][1], modelView[3][2]);
		}
	}
}
