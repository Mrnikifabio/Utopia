 #include <iostream>
#include "utopia.h"
#include "UNode.h"
#include "UMesh.h"
#include "UCamera.h"

using namespace utopia;

int main()
{
	/*Utopia::getInstance().init();
	Utopia::getInstance().setTestCallBack();
	Utopia::getInstance().mainLoop();
	Utopia::getInstance().free();*/

	auto rootShared = std::make_shared<UNode>("root");
	auto firstChild = rootShared->addChild(std::make_shared<UNode>("first child"));
	auto secondChild = rootShared->addChild(std::make_shared<UNode>("second child"));

	for (std::weak_ptr<UNode> child : rootShared->getChildren())
	{
		child.lock()->setModelView(child.lock()->getModelView() * glm::mat4(1));
		std::cout << child.lock()->getName() << std::endl;
	}

	for (auto& child : rootShared->getChildren())
	{
		child->setModelView(child->getModelView() * glm::mat4(1));
		std::cout << child->getName() << std::endl;
	}

	secondChild.lock()->addChild(std::make_shared<UNode>("giotto"));
	assert(rootShared->isChildPresent(3));

	secondChild.reset();

	auto secChildAnotherWeakPointer = rootShared->getChild(1);
	assert(!secChildAnotherWeakPointer.expired());
	
	auto neph = secChildAnotherWeakPointer.lock()->addChild(std::make_shared<UNode>("nephew"));

	auto secChildShared = rootShared->detachChild(1);

	std::cout << secChildAnotherWeakPointer.use_count() << std::endl;
	std::cout << secChildShared.use_count() << std::endl;

	rootShared.reset();
	assert(!secChildAnotherWeakPointer.expired());
	assert(firstChild.expired());

	return 0;
}