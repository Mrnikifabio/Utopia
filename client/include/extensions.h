#pragma once

#include "UNode.h"
#include <memory>
#include "UCamera.h"

namespace client {

	namespace extensions {

		std::shared_ptr<utopia::UNode> findGameObjectByName(std::shared_ptr<utopia::UNode> root, const std::string& name);
		void findGameObjectsByNameIntermediate(std::vector<std::shared_ptr<utopia::UNode>> vector, std::shared_ptr<utopia::UNode> root, const std::string& name);
		std::vector<std::shared_ptr<utopia::UNode>> findGameObjectsByName(std::shared_ptr<utopia::UNode> root, const std::string& name);
		glm::vec3 getLocalPosition(std::shared_ptr<utopia::UNode> node);
		glm::vec3 getWorldPosition(std::shared_ptr<utopia::UNode> node);

	}
}
