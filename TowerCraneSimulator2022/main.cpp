#include <iostream>
#include "utopia.h"

int main()
{
	Utopia::getInstance().init();
	Utopia::getInstance().setTestCallBack();
	Utopia::getInstance().mainLoop();
	Utopia::getInstance().free();
	std::cout << "Terminate" << std::endl;
}
