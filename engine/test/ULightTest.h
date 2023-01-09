#pragma once
#include "Test.h"

namespace utopia_test {
	class ULightTest : public Test
	{
	private:
		void execute() override;
        void initAndGetIDsTest();

        void setAndGetAmbient(const glm::vec4& ambient);
        void setAndGetDiffuse(const glm::vec4& diffuse);
        void setAndGetSpecular(const glm::vec4& specular);
        void setAndGetGlobalAmbient(const glm::vec4& gAmbient);

	public:
		ULightTest(const std::string& name);
	};
}