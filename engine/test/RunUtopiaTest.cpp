#include "Test.h"
#include "TestRunner.h"
#include <iostream>
#include <cassert>
#include <memory>

#include "Utopia.h"
#include "UObjectTest.h"
#include "OVOFactoryTest.h"
#include "UNodeTest.h"
#include "UCameraTest.h"
#include "ULightTest.h"

namespace utopia_test {

    void launchTest(const std::string& arg)
    {
        if(arg != "HEADLESS")
            utopia::Utopia::getInstance().init();

        TestRunner::getInstance().addTest(std::unique_ptr<UObjectTest>(new UObjectTest("UObjectTest")));
        TestRunner::getInstance().addTest(std::unique_ptr<UNodeTest>(new UNodeTest("UNodeTest")));
        TestRunner::getInstance().addTest(std::unique_ptr<OVOFactoryTest>(new OVOFactoryTest("OVOFactoryTest")));
		TestRunner::getInstance().addTest(std::unique_ptr<UCameraTest>(new UCameraTest("UCameraTest")));
		TestRunner::getInstance().addTest(std::unique_ptr<ULightTest>(new ULightTest("ULightTest")));

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "UTOPIA TESTS" << std::endl;
        TestRunner::getInstance().run(arg);
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "All tests have passed!" << std::endl;

        if (arg != "HEADLESS")
            utopia::Utopia::getInstance().free();
    }

}
int main(int argc, char*argv[])
{
    if (argc > 1)
        utopia_test::launchTest(argv[1]);
    else
        utopia_test::launchTest("");

    return 0;
}
