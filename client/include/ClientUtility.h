#include "UNode.h"
#include <memory>

#pragma once
namespace client
{

	class ClientUtility
	{

	private:
		ClientUtility();
		~ClientUtility();

	public:

		static ClientUtility& getInstance();

		ClientUtility(ClientUtility const&) = delete;
		void operator=(ClientUtility const&) = delete;

		std::shared_ptr<utopia::UNode> findGameObjectByName(std::shared_ptr<utopia::UNode> root, const std::string& name);
	
		glm::vec3 getLocalPosition(std::shared_ptr<utopia::UNode> node)
		{
			auto modelView = node->getModelView();
			return glm::vec3(modelView[3][0], modelView[3][1], modelView[3][2]);
		}
	};


}

