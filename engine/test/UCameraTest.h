#pragma once
#include "Test.h"

namespace utopia_test {
	class UCameraTest : public Test
	{
	private:
		void execute(const std::string& arg) override;
		void getCameraMatrixTest();	

		void setAndGetFarTest();
		void setAndGetNearTest();

		void setAndGetMainCameraTest();

	public:
		UCameraTest(const std::string& name);
	};
}