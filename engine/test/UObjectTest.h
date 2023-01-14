#pragma once
#include "Test.h"

namespace utopia_test {
	class UObjectTest : public Test
	{
	private:
		void execute(const std::string& arg) override;
		void IDTest(const std::string& arg);
		void nameTest();
	public:
		UObjectTest(const std::string& name);
	};
}