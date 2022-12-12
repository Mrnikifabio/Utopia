#include <iostream>
#include "utopia.h"
#include "UNode.h"
#include "UMesh.h"
#include "UCamera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

void keyboardCallback(unsigned char key, int mouseX, int mouseY);
void closeCallback();


using namespace utopia;

float g = 0.0f;

auto rootShared = std::make_shared<UNode>("root");
auto cube = rootShared->addChild(std::make_shared<UMesh>("cube"));
glm::vec3 cameraPosition;
std::shared_ptr<UCamera> camera;

int main()
{
	cameraPosition = glm::vec3(0, 0, -25);
	Utopia::getInstance().init();
	Utopia::getInstance().enableDepth();
	Utopia::getInstance().enableCullFace();
	Utopia::getInstance().setKeyboardCallback(keyboardCallback);
	Utopia::getInstance().setCloseCallback(closeCallback);


	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -25.0f));
	glm::mat4 rotation = glm::rotate(translation, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	cube.lock()->setModelView(rotation);

	auto another_cube = cube.lock()->addChild(std::make_shared<UMesh>("anotherCube"));
	another_cube.lock()->setModelView(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5, 0.0f)));

	camera = std::make_shared<UCamera>("firstCamera");

	camera->lookAt(cameraPosition, glm::vec3(0,0,-25), glm::vec3(0, 1, 0));

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
    switch (key)
    {
    
    case 'w':
		Utopia::getInstance().enableWireFrameMode();
		break;
	case 's':
		Utopia::getInstance().enableSolidMode();
		break;
	case 'r':
		cameraPosition += 0.05f * glm::vec3(0, 0, -25);
		camera->lookAt(cameraPosition, glm::vec3(0, 0, -25), glm::vec3(0, 1, 0));
		std::cout << "camera" << std::endl;
		std::cout << glm::to_string(camera->getModelView()) << std::endl;
		break;
	case 't':	
		cameraPosition -= 0.05f * glm::vec3(0, 0, -25);
		camera->lookAt(cameraPosition, glm::vec3(0, 0, -25), glm::vec3(0, 1, 0));
		std::cout << "camera" << std::endl;
		std::cout << glm::to_string(camera->getModelView()) << std::endl;
		break;
	
	}

}
void closeCallback()
{
	std::cout << "Close" << std::endl;
	Utopia::getInstance().free();
}
