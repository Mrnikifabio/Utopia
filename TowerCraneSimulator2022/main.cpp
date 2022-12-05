 #include <iostream>
#include "utopia.h"
#include "UNode.h"
#include "UMesh.h"
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
		mesh.lock()->render();



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
