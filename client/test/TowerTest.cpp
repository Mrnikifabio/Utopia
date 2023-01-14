#include "TowerTest.h"
#include "Tower.h"
#include <iostream>
#include <cassert>

void utopia_test::TowerTest::execute(const std::string& arg)
{
	Test::execute(arg);

	setAndGetRootTest();
	std::cout << "setAndGetRootTest passed" << std::endl;

	setAndGetTowerTest();
	std::cout << "setAndGetTowerTest passed" << std::endl;

	setAndGetHookTest();
	std::cout << "setAndGetHookTest passed" << std::endl;

	setAndGetFisicalHookTest();
	std::cout << "setAndGetFisicalHookTest passed" << std::endl;
	
	setAndGetCableTest();
	std::cout << "setAndGetCableTest passed" << std::endl;
	
}

void utopia_test::TowerTest::setAndGetRootTest()
{
	auto tower = std::make_shared<client::Tower>();
	assert(tower->getRoot() == nullptr);

	auto testNode = std::make_shared<utopia::UNode>("testNode");
	tower->setRoot(testNode);

	assert(tower->getRoot() == testNode);
}

void utopia_test::TowerTest::setAndGetTowerTest()
{
	auto tower = std::make_shared<client::Tower>();
	assert(tower->getTower() == nullptr);

	auto testNode = std::make_shared<utopia::UNode>("testNode");
	tower->setTower(testNode);

	assert(tower->getTower() == testNode);
}

void utopia_test::TowerTest::setAndGetHookTest()
{
	auto tower = std::make_shared<client::Tower>();
	assert(tower->getHook() == nullptr);

	auto testNode = std::make_shared<utopia::UNode>("testNode");
	tower->setHook(testNode);

	assert(tower->getHook() == testNode);
}

void utopia_test::TowerTest::setAndGetFisicalHookTest()
{
	auto tower = std::make_shared<client::Tower>();
	assert(tower->getFisicalHook() == nullptr);

	auto testNode = std::make_shared<utopia::UNode>("testNode");
	tower->setFisicalHook(testNode);

	assert(tower->getFisicalHook() == testNode);
}

void utopia_test::TowerTest::setAndGetCableTest()
{
	auto tower = std::make_shared<client::Tower>();
	assert(tower->getCable() == nullptr);

	auto testNode = std::make_shared<utopia::UNode>("testNode");
	tower->setCable(testNode);

	assert(tower->getCable() == testNode);
}

utopia_test::TowerTest::TowerTest(const std::string& name) : Test(name)
{
}
