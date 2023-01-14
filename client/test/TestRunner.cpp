#include "TestRunner.h"

using namespace utopia_test;

TestRunner::TestRunner() {}

TestRunner::~TestRunner() = default;

void TestRunner::run(const std::string& arg)
{
	for (auto& test : m_test)
	{
		test->execute(arg);
		test.reset();
	}
	m_test.clear();
}

void TestRunner::addTest(std::unique_ptr<Test>&& test)
{
	m_test.push_back(std::move(test));
}

TestRunner& TestRunner::getInstance()
{
	static TestRunner m_instance;
	return m_instance;
}