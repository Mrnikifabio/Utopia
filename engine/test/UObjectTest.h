#pragma once
#include "Test.h"

namespace utopia_test {
	class UObjectTest : public Test
	{
	private:
		void execute() override;
		void IDTest();
		void nameTest();
	public:
		UObjectTest(const std::string& name);
	};
}