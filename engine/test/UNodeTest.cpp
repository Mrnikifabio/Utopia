#include "UNodeTest.h"
#include <cassert>
#include <iostream>

using namespace utopia_test;

void UNodeTest::execute()
{
	Test::execute();
	std::cout << "Example test" << std::endl;
	assert(true);
}

UNodeTest::UNodeTest(const std::string& name) : Test(name) {}
