#include <iostream>
#include "Utopia.h"
#include "UNode.h"
#include "UMesh.h"
#include "UPerspectiveCamera.h"
#include "UOrthographicCamera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "OVOFactory.h"
#include "UOmniLight.h"
#include "ClientUtility.h"
#include "USpecialKeys.h"
#include "Tower.h"
#include "BoxesManager.h"
#include "Box.h"

void keyboardCallback(unsigned char key, int mouseX, int mouseY);
void specialCallback(int key, int mouseX, int mouseY);

void closeCallback();

using namespace utopia;

float g = -25;
std::shared_ptr<UCamera> camera;

std::unique_ptr<client::Tower> tower = std::make_unique<client::Tower>();
std::unique_ptr <client::BoxesManager> boxesManager = std::make_unique<client::BoxesManager>();




int main()
{
	Utopia::getInstance().init();
	Utopia::getInstance().setBackgroundColor(glm::vec4(0.5f,1.f,1.f,1.f));
	Utopia::getInstance().enableDepth();
	Utopia::getInstance().enableCullFace();
	Utopia::getInstance().setKeyboardCallback(keyboardCallback);
	Utopia::getInstance().setSpecialCallback(specialCallback);
	Utopia::getInstance().setCloseCallback(closeCallback);

	camera = std::make_shared<UPerspectiveCamera>("perspCamera");
	camera->setFar(2000.0f);
	camera->setNear(0.1f);
	camera->setModelView(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 400.0f, 990.0f)));

	auto root = OVOFactory::getInstance().fromFile("assets/gru15.ovo");


	std::shared_ptr<UNode> hookNode;
	std::shared_ptr<UNode> towerNode;
	std::shared_ptr<UNode> fisicalHookNode;
	std::shared_ptr<UNode> cableNode;

	hookNode = client::ClientUtility::getInstance().findGameObjectByName(root, "hook");
	towerNode = client::ClientUtility::getInstance().findGameObjectByName(root, "tower");
	fisicalHookNode = client::ClientUtility::getInstance().findGameObjectByName(root, "fisicalHook");
	cableNode = client::ClientUtility::getInstance().findGameObjectByName(root,"cable");

	auto hookPoints = client::ClientUtility::getInstance().findGameObjectsByName(root, "hookPoint");
	std::cout << "hook points size: " << hookPoints.size() << std::endl;

	std::vector<std::shared_ptr<client::Box>> boxesVector;

	for (auto hookPoint : hookPoints)
	{
		auto box = std::make_shared<client::Box>();
		box->setHookPointNode(hookPoint);
		box->setTopNode(client::ClientUtility::getInstance().findGameObjectByName(hookPoint, "top"));
		box->setGroundNode(client::ClientUtility::getInstance().findGameObjectByName(hookPoint,"ground"));
		boxesVector.push_back(box);
	}


	tower->setTower(towerNode);
	tower->setHook(hookNode);
	tower->setFisicalHook(fisicalHookNode);
	tower->setCable(cableNode);
	tower->setRoot(root);

	tower->setFisicalHookLimitDown(-600.f);
	tower->setFisicalHookLimitUp(0.f);
	tower->setHookLimitbackward(-350.f);
	tower->setHookLimitforward(0.f);

	boxesManager->setBoxes(boxesVector);

	UCamera::setMainCamera(camera);
	Utopia::getInstance().get3DRenderPipeline().pass(root, glm::mat4(1));

	while (Utopia::getInstance().isRunning())
	{
		Utopia::getInstance().mainLoop();
		Utopia::getInstance().clear();
		Utopia::getInstance().get3DRenderPipeline().render();
		Utopia::getInstance().get2DRenderPipeline().render();
		Utopia::getInstance().enableLighting();
		Utopia::getInstance().enableShadeModel();
		Utopia::getInstance().swap();
		//hookNode->setModelView(glm::rotate(hookNode->getModelView(), glm::radians(5.f), glm::vec3(0.f,1.f,0.f)));
	}

	std::cout << "Terminate" << std::endl;
}


void keyboardCallback(unsigned char key, int mouseX, int mouseY)
{
	glm::vec3 cameraNewPos = camera->getModelView() * glm::vec4(0,0,0,1);

	auto box = boxesManager->possibleBoxToHook(tower->getFisicalHook(), 150);

	switch (key)
    {

    case 'a':
		cameraNewPos.x -= 5.00f;
		break;
	case 'd':
		cameraNewPos.x += 5.00f;
		break;
	case 's':
		cameraNewPos.z += 5.00f;
		break;
	case 'w':
		cameraNewPos.z -= 5.00f;
		break;
	case 'r':
		cameraNewPos.y += 5.00f;
		break;
	case 'f':
		cameraNewPos.y -= 5.00f;
		break;

	case 'z':
		Utopia::getInstance().enableSolidMode();
		break;

	case 'x':
		Utopia::getInstance().enableWireFrameMode();
		break;

		/*
	case '1':
		Utopia::getInstance().updateAllTexturesParameteri(UTexture::enableNearestFilter);
		break;
	case '2':
		Utopia::getInstance().updateAllTexturesParameteri(UTexture::enableNearestBipmapNearestFilter);
		break;
	case '3':
		Utopia::getInstance().updateAllTexturesParameteri(UTexture::enableLinearFilter);
		break;
	case '4':
		Utopia::getInstance().updateAllTexturesParameteri(UTexture::enableLinearBipmapNearestFilter);
		break;
	case '5':
		Utopia::getInstance().updateAllTexturesParameteri(UTexture::enableLinearBipmapLinearFilter);
		break;
		*/

	case 'e':
		if (box != nullptr && !tower->isHooking())
		{
			tower->take(box);
		}
		else
		{
			if (tower->isHooking())
			{
				tower->release();
				std::cout << "is Hooking: " << std::endl;
			}
			else
			{
				std::cout << "is not Hooking: " << std::endl;
			}
		}
		
		break;


	case '-':
		tower->moveFisicalHookUpDown(-3.f);
		break;
	case '+':
		tower->moveFisicalHookUpDown(+3.f);
		break;
	}

	camera->setModelView(glm::translate(glm::mat4(1), cameraNewPos));

	std::cout << "camera" << std::endl;
	std::cout << glm::to_string(client::ClientUtility::getInstance().getLocalPosition(camera)) << std::endl;

}

void specialCallback(int key, int mouseX, int mouseY)
{
	std::cout << "[key pressed]" << std::endl;

	switch (key)
	{

	case (int)USpecialKeys::KEY_LEFT:
		tower->rotateTower(glm::radians(5.f));
		break;

	case (int)USpecialKeys::KEY_RIGHT:
		tower->rotateTower(glm::radians(-5.f));
		break;

	case (int)USpecialKeys::KEY_UP:
		tower->moveHookBackwardForward(5.f);
		break;

	case (int)USpecialKeys::KEY_DOWN:
		tower->moveHookBackwardForward(-5.f);
		break;

	}
}

void closeCallback()
{
	std::cout << "Close" << std::endl;
	Utopia::getInstance().free();
}
