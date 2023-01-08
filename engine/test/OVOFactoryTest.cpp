#include "OVOFactoryTest.h"
#include "OVOFactory.h"
#include "Utopia.h"
#include "UMesh.h"
#include <cassert>
#include <iostream>

using namespace utopia_test;

void OVOFactoryTest::execute()
{
	Test::execute();

	fromFileTest();
	std::cout << "fromFileTest passed" << std::endl;

	badFileTest();
	std::cout << "badFileTest passed" << std::endl;

	nonExistentFileTest();
	std::cout << "nonExistentFileTest passed" << std::endl;
}

void OVOFactoryTest::badFileTest()
{
	using namespace utopia;
	try
	{
		auto test = OVOFactory::getInstance().fromFile("bin/Test/assetsassets/circuit.dds");
	}
	catch (std::runtime_error&)
	{
		//the exception has been catched, therefore it has been thrown and the test is considered passed
		assert(true);
	}
}

void OVOFactoryTest::nonExistentFileTest()
{
	using namespace utopia;

	try
	{
		auto test = OVOFactory::getInstance().fromFile("nonexistent");
	}
	catch (std::runtime_error&)
	{
		//the exception has been catched, therefore it has been thrown and the test is considered passed
		assert(true);
	}
}

void OVOFactoryTest::fromFileTest()
{
	using namespace utopia;
	auto scene = OVOFactory::getInstance().fromFile("assets/simple3dScene.ovo");

	assert(!Utopia::getInstance().getMaterialByName("14 - Default").expired());
	assert(!Utopia::getInstance().getMaterialByName("08 - Default").expired());
	assert(!Utopia::getInstance().getMaterialByName("13 - Default").expired());
	assert(!Utopia::getInstance().getMaterialByName("07 - Default").expired());
	assert(!Utopia::getInstance().getMaterialByName("02 - Default").expired());
	assert(!Utopia::getInstance().getMaterialByName("03 - Default").expired());
	assert(!Utopia::getInstance().getMaterialByName("09 - Default").expired());
	assert(!Utopia::getInstance().getMaterialByName("Material #0").expired());
	
	assert(Utopia::getInstance().getTextureByName("assets/circuit.dds") != nullptr);
	assert(Utopia::getInstance().getTextureByName("assets/bricks.dds") != nullptr);
	assert(Utopia::getInstance().getTextureByName("assets/pave.dds") != nullptr);
	
	assert(scene->getName() == "[root]");

	for (unsigned int i = 0; i < scene->getChildCount(); i++)
	{
		auto child = scene->getChild(i);
		
		if(i == 0)
			assert(child.lock()->getName() == "Box001");
		else if (i == 9) //picking a random element in the list to see if everything was correctly loaded
		{
			assert(child.lock()->getName() == "Pyramid001");
			auto mesh = std::dynamic_pointer_cast<UMesh>(child.lock());
			assert(mesh->getMaterial().lock()->getName() == "09 - Default");
			assert(child.lock()->getChild(0).lock()->getName() == "Pyramid002");
		}
	}
}

OVOFactoryTest::OVOFactoryTest(const std::string& name) : Test(name) {}
