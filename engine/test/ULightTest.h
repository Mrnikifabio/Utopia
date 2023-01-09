#pragma once
#include "Test.h"

namespace utopia_test {
	class ULightTest : public Test
	{
	private:
		void execute() override;
        void initAndGetIDsTest();

        void setAndGetAmbient();
        void setAndGetDiffuse();
        void setAndGetSpecular();
        void setAndGetGlobalAmbient();

	public:
		ULightTest(const std::string& name);
	};
}