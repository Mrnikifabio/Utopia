#include <iostream>
#include "utopia.h"
#include "UNode.h"
#include "UMesh.h"
#include "UPerspectiveCamera.h"
#include "UOrthographicCamera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

void keyboardCallback(unsigned char key, int mouseX, int mouseY);
void closeCallback();


using namespace utopia;

float g = -25;
std::shared_ptr<UCamera> camera;
auto rootShared = std::make_shared<UNode>("root");
auto cube = rootShared->addChild(std::make_shared<UMesh>("cube"));


int main()
{
	Utopia::getInstance().init();
	Utopia::getInstance().enableDepth();
	Utopia::getInstance().enableCullFace();
	Utopia::getInstance().setKeyboardCallback(keyboardCallback);
	Utopia::getInstance().setCloseCallback(closeCallback);

	

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, g));
	glm::mat4 rotation = glm::rotate(translation, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	cube.lock()->setModelView(rotation);

	auto another_cube = cube.lock()->addChild(std::make_shared<UMesh>("anotherCube"));
	another_cube.lock()->setModelView(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5, 0.0f)));

	//camera = std::make_shared<UOrthographicCamera>("orthoCamera");
	camera = std::make_shared<UPerspectiveCamera>("orthoCamera");
	rootShared->addChild(camera);

	auto ccc = rootShared->addChild(std::make_shared<UMesh>("here we go"));
	ccc.lock()->setModelView(glm::translate(glm::mat4(1), glm::vec3(-10,0,g)));


	UCamera::setMainCamera(camera);
	Utopia::getInstance().getRenderPipeline().pass(rootShared, glm::mat4(1));

	while (Utopia::getInstance().isRunning())
	{
		Utopia::getInstance().mainLoop();
		Utopia::getInstance().clear();
		Utopia::getInstance().getRenderPipeline().render();
		Utopia::getInstance().display();
		Utopia::getInstance().swap();
	}

	std::cout << "Terminate" << std::endl;
}


void keyboardCallback(unsigned char key, int mouseX, int mouseY)
{
	glm::vec3 cameraNewPos = camera->getModelView() * glm::vec4(0,0,0,1);

	switch (key)
    {
    
    case 'a':
		cameraNewPos.x -= 1.00f;
		camera->setModelView(glm::translate(glm::mat4(1), cameraNewPos));
		break;
	case 'd':
		cameraNewPos.x += 1.00f;
		camera->setModelView(glm::translate(glm::mat4(1), cameraNewPos));
		break;
	case 's':
		cameraNewPos.z += 1.00f;
		camera->setModelView(glm::translate(glm::mat4(1), cameraNewPos));
		break;
	case 'w':
		cameraNewPos.z -= 1.00f;
		camera->setModelView(glm::translate(glm::mat4(1), cameraNewPos));
		break;
	case 't':
		cube.lock()->setModelView(glm::rotate(glm::translate(glm::mat4(1), glm::vec3(0, 0, g += 1.00f)), glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		std::cout << "cube" << std::endl;
		std::cout << glm::to_string(cube.lock()->getModelView()) << std::endl;
		break;
	case 'y':
		cube.lock()->setModelView(glm::rotate(glm::translate(glm::mat4(1), glm::vec3(0, 0, g -= 1.00f)) , glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		std::cout << "cube" << std::endl;
		std::cout << glm::to_string(cube.lock()->getModelView()) << std::endl;
		break;
	}

	std::cout << "camera" << std::endl;
	std::cout << glm::to_string(camera->getModelView()) << std::endl;

}
void closeCallback()
{
	std::cout << "Close" << std::endl;
	Utopia::getInstance().free();
}
