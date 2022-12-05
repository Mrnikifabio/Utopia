#include <iostream>
#include "utopia.h"

void keyboardCallback(unsigned char key, int mouseX, int mouseY);
void closeCallback();


int main()
{
	Utopia::getInstance().init();
	Utopia::getInstance().enableDepth();
	Utopia::getInstance().enableCullFace();
	Utopia::getInstance().setKeyboardCallback(keyboardCallback);
	Utopia::getInstance().setCloseCallback(closeCallback);

	while (Utopia::getInstance().isRunning())
	{
		Utopia::getInstance().mainLoop();
		Utopia::getInstance().clear();
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
