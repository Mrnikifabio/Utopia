#include "Test.h"
#include "ConcreteTestExample.h"
#include "TestRunner.h"
#include <iostream>
#include <cassert>
#include <memory>

namespace utopia_test {

    void launchTest()
    {
        TestRunner::getInstance().addTest(std::make_unique<ConcreteTestExample>("test1"));

        std::cout << "-------------" << std::endl;
        std::cout << "TOWERCRANESIMULATOR2022 TESTS" << std::endl;
        TestRunner::getInstance().run();
        std::cout << "-------------" << std::endl;
        std::cout << "All tests have been finished!" << std::endl;
    }

}
int main()
{
    utopia_test::launchTest();
    return 0;
}
