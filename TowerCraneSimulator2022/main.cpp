#include <iostream>
#include "utopia.h"
#include "UNode.h"
#include "UMesh.h"
#include "UPerspectiveCamera.h"
#include "UOrthographicCamera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "OVOFactory.h"

void keyboardCallback(unsigned char key, int mouseX, int mouseY);
void closeCallback();


using namespace utopia;

float g = -25;
std::shared_ptr<UCamera> camera;


int main()
{
	Utopia::getInstance().init();
	Utopia::getInstance().enableDepth();
	Utopia::getInstance().enableCullFace();
	Utopia::getInstance().setKeyboardCallback(keyboardCallback);
	Utopia::getInstance().setCloseCallback(closeCallback);

	

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, g));
	glm::mat4 rotation = glm::rotate(translation, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	//camera = std::make_shared<UOrthographicCamera>("orthoCamera");
	camera = std::make_shared<UPerspectiveCamera>("orthoCamera", glm::radians(45.0f), 1.0f, 600.0f);
	camera->setCameraPosition(glm::vec3(0, 4.0f, 0));

	auto root = OVOFactory::getInstance().fromFile("simple3dScene.ovo");
	root->addChild(camera);


	UCamera::setMainCamera(camera);
	Utopia::getInstance().getRenderPipeline().pass(root, glm::mat4(1));

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
	glm::vec3 cameraNewPos = glm::vec3(camera->getCameraPosition());

	switch (key)
    {
    
    case 'a':
		cameraNewPos.x -= 1.00f;
		camera->setCameraPosition(cameraNewPos);
		break;
	case 'd':
		cameraNewPos.x += 1.00f;
		camera->setCameraPosition(cameraNewPos);
		break;
	case 's':
		cameraNewPos.z += 1.00f;
		camera->setCameraPosition(cameraNewPos);
		break;
	case 'w':
		cameraNewPos.z -= 1.00f;
		camera->setCameraPosition(cameraNewPos);
		break;
	case 'r':
		cameraNewPos.y += 1.00f;
		camera->setCameraPosition(cameraNewPos);
		break;
	case 'f':
		cameraNewPos.y -= 1.00f;
		camera->setCameraPosition(cameraNewPos);
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
