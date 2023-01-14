#include "BoxesManagerTest.h"
#include "BoxesManager.h"
#include "Box.h"
#include "UNode.h"
#include <iostream>

void utopia_test::BoxesManagerTest::execute(const std::string& arg)
{
	Test::execute(arg);
	
	setAndGetBoxesTest();
	std::cout << "setAndGetBoxesTest passed" << std::endl;
}

void utopia_test::BoxesManagerTest::setAndGetBoxesTest()
{
	auto boxes = std::vector<std::shared_ptr<client::Box>>();
	auto box = std::make_shared<client::Box>();
	boxes.push_back(box);
	auto boxesManager = std::make_shared<client::BoxesManager>();
	assert(boxesManager->getBoxes().size() == 0);

	boxesManager->setBoxes(boxes);
	
	assert(boxesManager->getBoxes().size() == 1);

}

utopia_test::BoxesManagerTest::BoxesManagerTest(const std::string& name) : Test(name) {}
