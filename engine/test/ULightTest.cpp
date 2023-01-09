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

	setAndGetAmbient();
	std::cout << "set and get ambient passed" << std::endl;

	setAndGetDiffuse();
	std::cout << "set and get diffuse passed" << std::endl;

	setAndGetSpecular();
	std::cout << "set and get specular passed" << std::endl;

	setAndGetGlobalAmbient();
	std::cout << "set and get global ambient passed" << std::endl;

}

void ULightTest::initAndGetIDsTest()
{
	using namespace utopia;

	ULight::initIDs();
	auto light = ULight("light");
	assert(light.getLightID() == 0);
}

void ULightTest::setAndGetAmbient()
{
	using namespace utopia;

	auto light = ULight("light");
	light.setAmbient(glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
	assert(light.getAmbient() == glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
}

void ULightTest::setAndGetDiffuse()
{
	using namespace utopia;

	auto light = ULight("light");
	light.setDiffuse(glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
	assert(light.getDiffuse() == glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
}

void ULightTest::setAndGetSpecular()
{
	using namespace utopia;

	auto light = ULight("light");
	light.setSpecular(glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
	assert(light.getSpecular() == glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
}

void ULightTest::setAndGetGlobalAmbient()
{
	using namespace utopia;

	auto light = ULight("light");
	light.setGlobalAmbient(glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
	assert(light.getGlobalAmbient() == glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
}