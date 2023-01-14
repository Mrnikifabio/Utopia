#pragma once
#include "Test.h"

namespace utopia_test {
	class TowerTest : public Test
	{
	private:
		void execute(const std::string& arg) override;
		void setAndGetRootTest();
		void setAndGetTowerTest();
		void setAndGetHookTest();
		void setAndGetFisicalHookTest();
		void setAndGetCableTest();
		
	public:
		TowerTest(const std::string& name);
	};
}