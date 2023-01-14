#pragma once
#include "Test.h"

namespace utopia_test {
	class ClientUtilityTest : public Test
	{
	private:
		void execute(const std::string& arg) override;
		void findGameObjectByNameTest();
		void findGameObjectsByNameTest();
		void getLocalPositionTest();
		void getWorldPositionTest();

	public:
		ClientUtilityTest(const std::string& name);
	};
}