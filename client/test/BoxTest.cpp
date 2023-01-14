#include "BoxTest.h"
#include "Box.h"
#include "UNode.h"
#include <cassert>
#include <iostream>


using namespace utopia_test;

void BoxTest::execute(const std::string& arg)
{
	Test::execute(arg);

	setAndGetHookPointNode();
	std::cout << "setAndGetHookPointNode passed" << std::endl;

	setAndGetGroundNode();
	std::cout << "setAndGetGroundNode passed" << std::endl;

	setAndGetTopNode();
	std::cout << "setAndGetTopNode passed" << std::endl;

}

void utopia_test::BoxTest::setAndGetHookPointNode()
{
	std::unique_ptr<client::Box> box = std::unique_ptr<client::Box>(new client::Box());
	assert(box->getHookPointNode() == nullptr);

	auto testNode = std::make_shared<utopia::UNode>("testNode");
	box->setHookPointNode(testNode);

	assert(box->getHookPointNode() == testNode);

}

void utopia_test::BoxTest::setAndGetGroundNode()
{
	std::unique_ptr<client::Box> box = std::unique_ptr<client::Box>(new client::Box());
	assert(box->getGroundNode() == nullptr);


	auto testNode = std::make_shared<utopia::UNode>("testNode");
	box->setGroundNode(testNode);

	assert(box->getGroundNode() == testNode);
}

void utopia_test::BoxTest::setAndGetTopNode()
{
	std::unique_ptr<client::Box> box = std::unique_ptr<client::Box>(new client::Box());
	assert(box->getTopNode() == nullptr);


	auto testNode = std::make_shared<utopia::UNode>("testNode");
	box->setTopNode(testNode);

	assert(box->getTopNode() == testNode);
}

utopia_test::BoxTest::BoxTest(const std::string& name) : Test(name) {}
