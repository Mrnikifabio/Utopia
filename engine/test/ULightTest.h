#pragma once
#include "Test.h"

namespace utopia_test {
	class ULightTest : public Test
	{
	private:
		void execute(const std::string& arg) override;
        void initAndGetIDsTest();

        void setAndGetAmbient();
        void setAndGetDiffuse();
        void setAndGetSpecular();
        void setAndGetGlobalAmbient();
		
		void setAndGetConstantAttenuation();
		void setAndGetLinearAttenuation();
		void setAndGetQuadraticAttenuation();
		
		void setAndGetCutoff();
		void setAndGetDirection();

	public:
		ULightTest(const std::string& name);
	};
}