#include "UNodeTest.h"
#include "UNode.h"
#include "UPerspectiveCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cassert>
#include <iostream>

using namespace utopia_test;

UNodeTest::UNodeTest(const std::string& name) : Test(name) {}

void UNodeTest::execute(const std::string& arg)
{
	Test::execute(arg);

	modelViewTest();
	std::cout << "modelViewTest passed" << std::endl;
	
	childTest();
	std::cout << "childTest passed" << std::endl;

	parentTest();
	std::cout << "parentTest passed" << std::endl;

	childrenTest();
	std::cout << "childrenTest passed" << std::endl;

	finalWorldCoordinatesTest();
	std::cout << "finalWorldCoordinatesTest passed" << std::endl;
}


void UNodeTest::modelViewTest()
{
	using namespace utopia;

	auto node = std::unique_ptr<UNode>(new UNode(""));
	assert(node->getModelView() == glm::mat4(1));

	node->setModelView(glm::translate(glm::mat4(1), glm::vec3(1,2,3)));
	assert(node->getModelView() == glm::translate(glm::mat4(1), glm::vec3(1, 2, 3)));
}

void UNodeTest::childTest()
{
	using namespace utopia;

	auto node = std::shared_ptr<UNode>(new UNode("parent"));
	assert(node->getParent() == nullptr);

	auto child1 = std::shared_ptr<UNode>(new UNode("child1"));

	node->addChild(child1);
	node->addChild(std::unique_ptr<UNode>(new UNode("child2")));
	node->addChild(std::unique_ptr<UNode>(new UNode("child3")));


	auto otherNode = std::shared_ptr<UNode>(new UNode("otherParent"));
	auto otherParentChildren = std::shared_ptr<UNode>(new UNode("otherParentChildren"));
	otherNode->addChild(otherParentChildren);

	try
	{
		node->addChild(node);
		assert(false);
	}
	catch (std::runtime_error&)
	{
		//the exception has been catched, therefore it has been thrown and the test is considered passed
		assert(true);
	}


	try
	{
		node->addChild(child1);
		assert(false);
	}
	catch (std::runtime_error&)
	{
		//the exception has been catched, therefore it has been thrown and the test is considered passed
		assert(true);
	}

	try
	{
		node->addChild(otherParentChildren);
		assert(false);
	}
	catch (std::runtime_error&)
	{
		//the exception has been catched, therefore it has been thrown and the test is considered passed
		assert(true);
	}

	assert(node->getChildCount() == 3);

	auto c1 = node->detachChild(1);
	assert(node->getChildCount() == 2);
	assert(c1->getName() == "child2");

	try
	{
		node->detachChild(999);
		assert(false);
	}
	catch (std::out_of_range&)
	{
		//the exception has been catched, therefore it has been thrown and the test is considered passed
		assert(true);
	}

	try
	{
		node->detachChildById(999);
		assert(false);
	}
	catch (std::out_of_range&)
	{
		//the exception has been catched, therefore it has been thrown and the test is considered passed
		assert(true);
	}

	try
	{
		node->getChild(999);
		assert(false);
	}
	catch (std::out_of_range&)
	{
		//the exception has been catched, therefore it has been thrown and the test is considered passed
		assert(true);
	}
}

void UNodeTest::parentTest()
{
	using namespace utopia;
	auto node = std::unique_ptr<UNode>(new UNode("parent"));
	assert(node->getParent() == nullptr);

	node->addChild(std::unique_ptr<UNode>(new UNode("child1")));
	auto child = node->getChild(0);
	assert(child.lock()->getParent()->getId() == node->getId());

	auto childDetached = node->detachChild(0);
	assert(childDetached->getParent() == nullptr);
}

void UNodeTest::childrenTest()
{
	using namespace utopia;
	auto node = std::unique_ptr<UNode>(new UNode("parent"));
	assert(node->getParent() == nullptr);

	node->addChild(std::unique_ptr<UNode>(new UNode("child1")));
	node->addChild(std::unique_ptr<UNode>(new UNode("child2")));
	node->addChild(std::unique_ptr<UNode>(new UNode("child3")));

	int i = 1;
	std::string name = "child";
	for (auto& child : node->getChildren())
		assert(child->getName() == (name + std::to_string(i++)));
}

void UNodeTest::finalWorldCoordinatesTest()
{
	using namespace utopia;
	auto camera = std::make_shared<UPerspectiveCamera>("testcamera");
	UCamera::setMainCamera(camera);

	auto translate = glm::translate(glm::mat4(1), glm::vec3(1,2,3));
	auto rotate = glm::rotate(glm::mat4(1), glm::radians(23.0f), glm::vec3(0, 1, 0));

	auto node = std::unique_ptr<UNode>(new UNode("root"));
	auto child1 = std::make_shared<UNode>("c1");
	child1->setModelView(rotate);
	auto child2 = std::make_shared<UNode>("c2");
	child2->setModelView(translate);

	node->addChild(child1);
	child1->addChild(child2);

	assert(child2->getFinalWorldCoordinates() == glm::mat4(1) * rotate * translate * glm::inverse(UCamera::getMainCamera().lock()->getModelView()));
	assert(child1->getFinalWorldCoordinates() == glm::mat4(1) * rotate * glm::inverse(UCamera::getMainCamera().lock()->getModelView()));
	assert(node->getFinalWorldCoordinates() == glm::mat4(1) * glm::inverse(UCamera::getMainCamera().lock()->getModelView()));
}