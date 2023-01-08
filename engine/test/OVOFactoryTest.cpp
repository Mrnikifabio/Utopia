#include "OVOFactoryTest.h"
#include <cassert>
#include <iostream>

using namespace utopia_test;

void OVOFactoryTest::execute()
{
	Test::execute();
	std::cout << "Example test" << std::endl;
	assert(true);
}

OVOFactoryTest::OVOFactoryTest(const std::string& name) : Test(name) {}
