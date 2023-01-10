#include "UObjectTest.h"
#include "UObject.h"
#include "UNode.h"
#include <memory>
#include <cassert>
#include <iostream>

using namespace utopia_test;

void UObjectTest::execute(const std::string& arg)
{
	Test::execute(arg);

	IDTest(arg);
	std::cout << "testID passed" << std::endl;

	nameTest();
	std::cout << "testName passed" << std::endl;
}

void UObjectTest::IDTest(const std::string& arg)
{
	using namespace utopia;

	if (arg != "HEADLESS")
	{
		//an UNode is istantiated since that UObject is actually abstract
		auto obj = std::unique_ptr<UObject>(new UNode(""));
		assert(obj->getId() == 3);
		auto obj1 = std::unique_ptr<UObject>(new UNode(""));
		assert(obj1->getId() == 4);
		obj.reset();
		auto obj2 = std::unique_ptr<UObject>(new UNode(""));
		assert(obj2->getId() == 5);
	}
	else
	{
		//an UNode is istantiated since that UObject is actually abstract
		auto obj = std::unique_ptr<UObject>(new UNode(""));
		assert(obj->getId() == 1);
		auto obj1 = std::unique_ptr<UObject>(new UNode(""));
		assert(obj1->getId() == 2);
		obj.reset();
		auto obj2 = std::unique_ptr<UObject>(new UNode(""));
		assert(obj2->getId() == 3);
	}

}

void UObjectTest::nameTest()
{
	using namespace utopia;
	//an UNode is istantiated since that UObject is actually abstract
	auto obj = std::unique_ptr<UObject>(new UNode("test"));
	assert(obj->getName() == "test");
	obj->setName("changeName");
	assert(obj->getName() == "changeName");
}

UObjectTest::UObjectTest(const std::string& name) : Test(name) {}
