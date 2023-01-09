#pragma once

#include "UNode.h"
#include <memory>

namespace client
{

	class ClientUtility
	{

	private:
		ClientUtility();
		~ClientUtility();

		void findGameObjectsByNameIntermediate(std::vector<std::shared_ptr<utopia::UNode>> vector, std::shared_ptr<utopia::UNode> root, const std::string& name);


	public:

		static ClientUtility& getInstance();

		ClientUtility(ClientUtility const&) = delete;
		void operator=(ClientUtility const&) = delete;

		std::shared_ptr<utopia::UNode> findGameObjectByName(std::shared_ptr<utopia::UNode> root, const std::string& name);

		std::vector<std::shared_ptr<utopia::UNode>> findGameObjectsByName(std::shared_ptr<utopia::UNode> root, const std::string& name);
	
		glm::vec3 getLocalPosition(std::shared_ptr<utopia::UNode> node);
		glm::vec3 getWorldPosition(std::shared_ptr<utopia::UNode> node);
		
	};


}

