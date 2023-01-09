#include "Test.h"
#include "TestRunner.h"
#include <iostream>
#include <cassert>
#include <memory>

#include "UObjectTest.h"
#include "OVOFactoryTest.h"
#include "UNodeTest.h"
#include "UCameraTest.h"
#include "ULightTest.h"

namespace utopia_test {

    void launchTest()
    {
        TestRunner::getInstance().addTest(std::unique_ptr<UObjectTest>(new UObjectTest("UObjectTest")));
        TestRunner::getInstance().addTest(std::unique_ptr<UNodeTest>(new UNodeTest("UNodeTest")));
        TestRunner::getInstance().addTest(std::unique_ptr<OVOFactoryTest>(new OVOFactoryTest("OVOFactoryTest")));
		// TestRunner::getInstance().addTest(std::unique_ptr<UCameraTest>(new UCameraTest("UCameraTest")));
		// TestRunner::getInstance().addTest(std::unique_ptr<ULightTest>(new ULightTest("ULightTest")));

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "UTOPIA TESTS" << std::endl;
        TestRunner::getInstance().run();
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "All tests have passed!" << std::endl;
    }

}
int main()
{
    utopia_test::launchTest();
    return 0;
}
