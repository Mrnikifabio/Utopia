#include "UObjectTest.h"
#include "UObject.h"
#include <cassert>
#include <iostream>

using namespace utopia_test;

void UObjectTest::execute()
{
	Test::execute();
	std::cout << "Example test" << std::endl;
	assert(true);
}

UObjectTest::UObjectTest(const std::string& name) : Test(name) {}
