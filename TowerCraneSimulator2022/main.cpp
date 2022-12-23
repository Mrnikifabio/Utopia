#include <iostream>
#include "utopia.h"
#include "UNode.h"
#include "UMesh.h"
#include "UPerspectiveCamera.h"
#include "UOrthographicCamera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "OVOFactory.h"
#include "UOmniLight.h"

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
	camera = std::make_shared<UPerspectiveCamera>("perspCamera");
	camera->setFar(600.0f);
	camera->setNear(0.1f);
	camera->setModelView(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 45.0f, 320.0f)));
	auto c = glm::vec3(0.0f, 0.0f, g);
	auto root = OVOFactory::getInstance().fromFile("simple3dScene.ovo");


	auto light = root->addChild(std::make_shared<UOmniLight>("light", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0x4000));
	light.lock()->setModelView(glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 4.0f, 0.0f)));

	root->addChild(camera);



	UCamera::setMainCamera(camera);
	Utopia::getInstance().getRenderPipeline().pass(root, glm::mat4(1));

	while (Utopia::getInstance().isRunning())
	{
		Utopia::getInstance().mainLoop();
		Utopia::getInstance().clear();
		Utopia::getInstance().getRenderPipeline().render();
		Utopia::getInstance().enableLighting();
		Utopia::getInstance().enableLight0();
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
		break;
	case 'd':
		cameraNewPos.x += 1.00f;
		break;
	case 's':
		cameraNewPos.z += 1.00f;
		break;
	case 'w':
		cameraNewPos.z -= 1.00f;
		break;
	case 'r':
		cameraNewPos.y += 1.00f;
		break;
	case 'f':
		cameraNewPos.y -= 1.00f;
		break;

	case 'z':
		Utopia::getInstance().enableSolidMode();
		break;

	case 'x':
		Utopia::getInstance().enableWireFrameMode();

		break;


	}

	camera->setModelView(glm::translate(glm::mat4(1), cameraNewPos));
	std::cout << "camera" << std::endl;
	std::cout << glm::to_string(camera->getModelView()) << std::endl;

}
void closeCallback()
{
	std::cout << "Close" << std::endl;
	Utopia::getInstance().free();
}
