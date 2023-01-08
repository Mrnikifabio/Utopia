#pragma once
#include "Test.h"

namespace utopia_test {
	class OVOFactoryTest : public Test
	{
	private:
		void execute() override;
	public:
		OVOFactoryTest(const std::string& name);
	};
}