#include "UCameraTest.h"
#include "UCamera.h"
#include "UOrthographicCamera.h"
#include "UPerspectiveCamera.h"
#include <cassert>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


using namespace utopia_test;

UCameraTest::UCameraTest(const std::string& name) : Test(name) {}

void UCameraTest::execute(const std::string& arg)
{
	Test::execute(arg);

	if(arg != "HEADLESS")
	{
		getCameraMatrixTest();
		std::cout << "get Camera matrix passed" << std::endl;
	}

	setAndGetFarTest();
	std::cout << "set and get far passed" << std::endl;

	setAndGetNearTest();
	std::cout << "set and get near passed" << std::endl;

	setAndGetMainCameraTest();
	std::cout << "set and get main Camera passed" << std::endl;
}

void UCameraTest::getCameraMatrixTest()
{
	using namespace utopia;

	auto camera = std::unique_ptr<UPerspectiveCamera>(new UPerspectiveCamera(""));
	assert(camera->getCameraMatrix() == glm::mat4(1));
}

void UCameraTest::setAndGetFarTest()
{
	using namespace utopia;

    auto camera = std::unique_ptr<UPerspectiveCamera>(new UPerspectiveCamera(""));
	camera->setFar(100);
	assert(camera->getFar() == 100);
}

void UCameraTest::setAndGetNearTest()
{
	using namespace utopia;

	auto camera = std::unique_ptr<UPerspectiveCamera>(new UPerspectiveCamera(""));
	camera->setNear(0.1f);
	assert(camera->getNear() == 0.1f);
}

void UCameraTest::setAndGetMainCameraTest()
{
	using namespace utopia;

	auto camera = std::make_shared<UPerspectiveCamera>("camera");
	UCamera::setMainCamera(camera);
	assert(UCamera::getMainCamera().lock()->getId() == camera->getId());
}
