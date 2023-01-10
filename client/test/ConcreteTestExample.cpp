#include "ConcreteTestExample.h"
#include <cassert>
#include <iostream>

using namespace utopia_test;

void utopia_test::ConcreteTestExample::execute(const std::string& arg)
{
	Test::execute(arg);
	std::cout << "Example test"<<std::endl;
	assert(true);
}

ConcreteTestExample::ConcreteTestExample(const std::string& name) : Test(name) {}
