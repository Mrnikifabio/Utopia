#include "ULightTest.h"
#include "ULight.h"
#include <cassert>
#include <iostream>

using namespace utopia_test;

ULightTest::ULightTest(const std::string& name) : Test(name) {}

void ULightTest::execute()
{
	Test::execute();

	initAndGetIDsTest();
	std::cout << "init IDs passed" << std::endl;

	setAndGetAmbient(glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
	std::cout << "set and get ambient passed" << std::endl;

	setAndGetDiffuse(glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
	std::cout << "set and get diffuse passed" << std::endl;

	setAndGetSpecular(glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
	std::cout << "set and get specular passed" << std::endl;

	setAndGetGlobalAmbient(glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
	std::cout << "set and get global ambient passed" << std::endl;

}

void ULightTest::initAndGetIDsTest()
{
	using namespace utopia;

	ULight::initIDs();
	auto light = ULight("light");
	assert(light.getLightID() == 0);
}

void ULightTest::setAndGetAmbient(const glm::vec4& ambient)
{
	using namespace utopia;

	auto light = ULight("light");
	light.setAmbient(ambient);
	assert(light.getAmbient() == ambient);
}

void ULightTest::setAndGetDiffuse(const glm::vec4& diffuse)
{
	using namespace utopia;

	auto light = ULight("light");
	light.setDiffuse(diffuse);
	assert(light.getDiffuse() == diffuse);
}

void ULightTest::setAndGetSpecular(const glm::vec4& specular)
{
	using namespace utopia;

	auto light = ULight("light");
	light.setSpecular(specular);
	assert(light.getSpecular() == specular);
}

void ULightTest::setAndGetGlobalAmbient(const glm::vec4& gAmbient)
{
	using namespace utopia;

	auto light = ULight("light");
	light.setGlobalAmbient(gAmbient);
	assert(light.getGlobalAmbient() == gAmbient);
}