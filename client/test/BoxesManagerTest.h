#pragma once
#include "Test.h"

namespace utopia_test {
	class BoxesManagerTest : public Test
	{
	private:
		void execute(const std::string& arg) override;
		void setAndGetBoxesTest();
		
	public:
		BoxesManagerTest(const std::string& name);

	};
}