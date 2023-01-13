#include "Test.h"
#include "TestRunner.h"
#include <iostream>
#include <memory>

using namespace utopia_test; 

void Test::execute(const std::string& arg)
{
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Executing test " << getName()<<std::endl;
}

Test::Test(const std::string& name)
{
	m_name = name;
}

const std::string& Test::getName() const
{
	return m_name;
}
