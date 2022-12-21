#include <iostream>
#include "utopia.h"
#include "UNode.h"
#include "UMesh.h"
#include "UOmniLight.h"
#include "USpotLight.h"
#include "UDirectionalLight.h"
#include "UCamera.h"
#include <glm/ext/matrix_transform.hpp>

void keyboardCallback(unsigned char key, int mouseX, int mouseY);
void closeCallback();


using namespace utopia;

int main()
{
	Utopia::getInstance().init();
	Utopia::getInstance().enableDepth();
	Utopia::getInstance().enableCullFace();
	Utopia::getInstance().setKeyboardCallback(keyboardCallback);
	Utopia::getInstance().setCloseCallback(closeCallback);

	auto rootShared = std::make_shared<UNode>("root");

	while (Utopia::getInstance().isRunning())
	{
		Utopia::getInstance().mainLoop();
		Utopia::getInstance().clear();


		auto mesh = rootShared->addChild(std::make_shared<UMesh>("cube"));
		

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -25.0f));
		glm::mat4 rotation = glm::rotate(translation, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		mesh.lock()->setModelView(rotation);

		auto another_cube = mesh.lock()->addChild(std::make_shared<UMesh>("anotherCube"));
		another_cube.lock()->setModelView(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5, 0.0f)));
		mesh.lock()->setModelView(mesh.lock()->getModelView()*glm::translate(glm::mat4(1.0f), glm::vec3(10, 0, 0)));

		
		// auto light = rootShared->addChild(std::make_shared<UOmniLight>("light", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(0.2f,0.2f,0.2f,1.0f), 0x4000));
		auto light = rootShared->addChild(std::make_shared<UDirectionalLight>("light", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f ,1.0f ,1.0f ,1.0f), 0x4000));
		// auto light = rootShared->addChild(std::make_shared<USpotLight>("light", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(0.2f,0.2f,0.2f,1.0f), 0x4000, 5.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
		light.lock()->setModelView(glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 4.0f, 0.0f)));
		
		
		rootShared->render();


		Utopia::getInstance().enableLighting();
		Utopia::getInstance().enableLight0();
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
    }

}
void closeCallback()
{
	std::cout << "Close" << std::endl;
	Utopia::getInstance().free();
}
