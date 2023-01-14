#include "ClientUtilityTest.h"
#include "ClientUtility.h"
#include "UNode.h"
#include <iostream>

void utopia_test::ClientUtilityTest::execute(const std::string& arg)
{
	Test::execute(arg);
	
	findGameObjectByNameTest();
	std::cout << "findGameObjectByNameTest passed" << std::endl;

	findGameObjectsByNameTest();
	std::cout << "findGameObjectsByNameTest passed" << std::endl;
	
	getLocalPositionTest();
	std::cout << "getLocalPositionTest passed" << std::endl;
	
	getWorldPositionTest();
	std::cout << "getWorldPositionTest passed" << std::endl;
	
}

void utopia_test::ClientUtilityTest::findGameObjectByNameTest()
{
	
	
	std::shared_ptr<utopia::UNode> root = std::make_shared<utopia::UNode>("root");
	std::shared_ptr<utopia::UNode> child = std::make_shared<utopia::UNode>("child");
	assert(client::ClientUtility::getInstance().findGameObjectByName(root, "child") == nullptr);
	
	root->addChild(child);
	
	assert(client::ClientUtility::getInstance().findGameObjectByName(root, "child") == child);

}

void utopia_test::ClientUtilityTest::findGameObjectsByNameTest()
{
	std::shared_ptr<utopia::UNode> root = std::make_shared<utopia::UNode>("root");
	std::shared_ptr<utopia::UNode> child1 = std::make_shared<utopia::UNode>("child");
	std::shared_ptr<utopia::UNode> child2 = std::make_shared<utopia::UNode>("child");
	std::shared_ptr<utopia::UNode> child3 = std::make_shared<utopia::UNode>("child");
	
	assert(client::ClientUtility::getInstance().findGameObjectsByName(root, "child").size() == 0);
	
	root->addChild(child1);
	root->addChild(child2);
	root->addChild(child3);

	assert(client::ClientUtility::getInstance().findGameObjectsByName(root, "child").size() == 3);
}

void utopia_test::ClientUtilityTest::getLocalPositionTest()
{
	std::shared_ptr<utopia::UNode> node = std::make_shared<utopia::UNode>("node");
	node->setModelView(glm::mat4(1.f));
	assert(client::ClientUtility::getInstance().getLocalPosition(node) == glm::vec3(0.f));
		
	glm::mat4 localMatrix = glm::mat4(1.0f);
	localMatrix[3][0] = 1.0f;
	localMatrix[3][1] = 2.0f;
	localMatrix[3][2] = 3.0f;
	
	node->setModelView(localMatrix);
	assert(client::ClientUtility::getInstance().getLocalPosition(node) == glm::vec3(1.0f, 2.0f, 3.0f));
}

void utopia_test::ClientUtilityTest::getWorldPositionTest()
{
	std::shared_ptr<utopia::UNode> root = std::make_shared<utopia::UNode>("root");
	std::shared_ptr<utopia::UNode> child = std::make_shared<utopia::UNode>("child");
	
	
	root->addChild(child);
	glm::mat4 localMatrixRoot = glm::mat4(1.0f);
	localMatrixRoot[3][0] = 1.0f;
	localMatrixRoot[3][1] = 1.0f;
	localMatrixRoot[3][2] = 1.0f;

	glm::mat4 localMatrixChild = glm::mat4(1.0f);
	localMatrixChild[3][0] = 2.0f;
	localMatrixChild[3][1] = 2.0f;
	localMatrixChild[3][2] = 2.0f;
	
	root->setModelView(localMatrixRoot);
	child->setModelView(glm::mat4(1.f));

	assert(client::ClientUtility::getInstance().getWorldPosition(child) == glm::vec3(1.0f, 1.0f, 1.0f));
	
	child->setModelView(localMatrixChild);
	
	assert(client::ClientUtility::getInstance().getWorldPosition(child) == glm::vec3(3.0f, 3.0f, 3.0f));
	
}

utopia_test::ClientUtilityTest::ClientUtilityTest(const std::string& name): Test(name) {}
