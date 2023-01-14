#include "Test.h"
#include "TestRunner.h"
#include "Utopia.h"
#include "BoxTest.h"
#include "BoxesManagerTest.h"
#include "ClientUtilityTest.h"
#include "TowerTest.h"
#include <string>
#include <iostream>
#include <cassert>
#include <memory>

namespace utopia_test {

    void launchTest(const std::string& arg)
    {
        if (arg != "HEADLESS")
            utopia::Utopia::getInstance().init();

		TestRunner::getInstance().addTest(std::unique_ptr<BoxTest>(new BoxTest("BoxTest")));
		TestRunner::getInstance().addTest(std::unique_ptr<BoxesManagerTest>(new BoxesManagerTest("BoxesManagerTest")));
		TestRunner::getInstance().addTest(std::unique_ptr<TowerTest>(new TowerTest("TowerTest")));
		TestRunner::getInstance().addTest(std::unique_ptr<ClientUtilityTest>(new ClientUtilityTest("ClientUtilityTest")));
        
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "TOWERCRANESIMULATOR2022 TESTS" << std::endl;
        TestRunner::getInstance().run(arg);
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "All tests have passed!" << std::endl;

        if (arg != "HEADLESS")
            utopia::Utopia::getInstance().free();
    }

}
int main(int argc, char *argv[])
{
    if(argc > 1)
        utopia_test::launchTest(argv[1]);
    else
        utopia_test::launchTest("");

    return 0;
}
