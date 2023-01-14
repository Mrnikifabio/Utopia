#pragma once
#include "Test.h"

namespace utopia_test {
	class OVOFactoryTest : public Test
	{
	private:
		void execute(const std::string& arg) override;
		void fromFileTest();
		void badFileTest();
		void nonExistentFileTest();
	public:
		OVOFactoryTest(const std::string& name);
	};
}