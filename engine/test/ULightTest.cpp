#include "ULightTest.h"
#include "ULight.h"
#include "USpotLight.h"
#include <gl/freeglut.h>
#include <cassert>
#include <iostream>

using namespace utopia_test;

ULightTest::ULightTest(const std::string& name) : Test(name) {}

void ULightTest::execute(const std::string& arg)
{
	Test::execute(arg);

	if (arg != "HEADLESS")
	{

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

		setAndGetConstantAttenuation();
		std::cout << "set and get constant attenuation passed" << std::endl;

		setAndGetLinearAttenuation();
		std::cout << "set and get linear attenuation passed" << std::endl;

		setAndGetQuadraticAttenuation();
		std::cout << "set and get quadratic attenuation passed" << std::endl;

		setAndGetCutoff();
		std::cout << "set and get cutoff passed" << std::endl;

		setAndGetDirection();
		std::cout << "set and get direction passed" << std::endl;

	}

}

void ULightTest::initAndGetIDsTest()
{
	using namespace utopia;

	//ULight::initIDs();
	auto light = std::unique_ptr<ULight>(new ULight("light"));
	std::cout << light->getLightID() << std::endl;
	assert(light->getLightID() == GL_LIGHT7);
}

void ULightTest::setAndGetAmbient()
{
	using namespace utopia;

	auto light = std::unique_ptr<ULight>(new ULight("light"));
	light->setAmbient(glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
	assert(light->getAmbient() == glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
}

void ULightTest::setAndGetDiffuse()
{
	using namespace utopia;

	auto light = std::unique_ptr<ULight>(new ULight("light"));
	light->setDiffuse(glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
	assert(light->getDiffuse() == glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
}

void ULightTest::setAndGetSpecular()
{
	using namespace utopia;

	auto light = std::unique_ptr<ULight>(new ULight("light"));
	light->setSpecular(glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
	assert(light->getSpecular() == glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
}

void ULightTest::setAndGetGlobalAmbient()
{
	using namespace utopia;

	auto light = std::unique_ptr<ULight>(new ULight("light"));
	light->setGlobalAmbient(glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
	assert(light->getGlobalAmbient() == glm::vec4(0.1f, 0.2f, 0.3f, 0.4f));
}

void ULightTest::setAndGetConstantAttenuation()
{
	using namespace utopia;

	auto light = std::unique_ptr<USpotLight>(new USpotLight("light", 0.1f, glm::vec3(0.1f, 0.2f, 0.3f)));
	light->setConstantAttenuation(0.1f);
	assert(light->getConstantAttenuation() == 0.1f);
}

void ULightTest::setAndGetLinearAttenuation()
{
	using namespace utopia;

	auto light = std::unique_ptr<USpotLight>(new USpotLight("light", 0.1f, glm::vec3(0.1f, 0.2f, 0.3f)));
	light->setLinearAttenuation(0.1f);
	assert(light->getLinearAttenuation() == 0.1f);
}

void ULightTest::setAndGetQuadraticAttenuation()
{
	using namespace utopia;

	auto light = std::unique_ptr<USpotLight>(new USpotLight("light", 0.1f, glm::vec3(0.1f, 0.2f, 0.3f)));
	light->setQuadraticAttenuation(0.1f);
	assert(light->getQuadraticAttenuation() == 0.1f);
}

void ULightTest::setAndGetCutoff()
{
	using namespace utopia;

	auto light = std::unique_ptr<USpotLight>(new USpotLight("light", 0.1f, glm::vec3(0.1f, 0.2f, 0.3f)));
	light->setCutoff(0.1f);
	assert(light->getCutoff() == 0.1f);
}

void ULightTest::setAndGetDirection()
{
	using namespace utopia;

	auto light = std::unique_ptr<USpotLight>(new USpotLight("light", 0.1f, glm::vec3(0.1f, 0.2f, 0.3f)));
	light->setDirection(glm::vec3(0.1f, 0.2f, 0.3f));
	assert(light->getDirection() == glm::vec3(0.1f, 0.2f, 0.3f));
}