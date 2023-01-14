#pragma once

#include "Test.h"
#include <vector>
#include <memory>

namespace utopia_test {
	class TestRunner
	{
	private:
		std::vector<std::unique_ptr<Test>> m_test;
		TestRunner();
		virtual ~TestRunner() noexcept;

	public:
		void run(const std::string& arg);
		void addTest(std::unique_ptr<Test>&& test);

		static TestRunner& getInstance();

		TestRunner(TestRunner const&) = delete;
		void operator=(TestRunner const&) = delete;
	};
}

