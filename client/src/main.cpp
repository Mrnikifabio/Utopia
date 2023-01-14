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
#include <UText.h>

using namespace utopia;

void keyboardCallback(unsigned char key, int mouseX, int mouseY);
void specialCallback(int key, int mouseX, int mouseY);
void passiveMotionCallback(int x, int y);
void fpsCounterCallback(int value);
void closeCallback();

int fpsCounter = 0;
int fpsToPrint = 0;
int maxAnisotropyLevel;

std::shared_ptr<UCamera> freeCamera;
std::shared_ptr<UCamera> fixedCamera;
std::shared_ptr<UCamera> towerCamera;
std::shared_ptr<UCamera> orthoCamera;


std::unique_ptr<client::Tower> tower = std::unique_ptr<client::Tower>(new client::Tower());
std::unique_ptr <client::BoxesManager> boxesManager = std::unique_ptr<client::BoxesManager>(new  client::BoxesManager());

float sensibility = 0.5f;

std::shared_ptr<UText> anisotLevelLabel = std::make_shared<UText>("anisotLevelLabel");
std::shared_ptr<UText> textureFilterModeLabel = std::make_shared<UText>("textureFilterMode");

int main()
{
	Utopia::getInstance().init();
	Utopia::getInstance().setBackgroundColor(glm::vec4(0.5f, 1.f, 1.f, 1.f));
	Utopia::getInstance().enableDepth();
	Utopia::getInstance().enableCullFace();
	Utopia::getInstance().enableShadeModel();
	Utopia::getInstance().setKeyboardCallback(keyboardCallback);
	Utopia::getInstance().setSpecialCallback(specialCallback);
	Utopia::getInstance().setCloseCallback(closeCallback);
	Utopia::getInstance().setPassiveMotionCallback(passiveMotionCallback);
	Utopia::getInstance().setTimer(1000, fpsCounterCallback, 0);

	auto _2DRenderPipeline = std::unique_ptr<U2DRenderPipeline>(new U2DRenderPipeline("2DRenderPipeline"));
	auto _3DRenderPipeline = std::unique_ptr<U3DRenderPipeline>(new U3DRenderPipeline("3DRenderPipeline"));
	auto shadowRenderPipeline = std::unique_ptr<U3DRenderPipeline>(new U3DRenderPipeline("shadowRenderPipeline"));

	maxAnisotropyLevel = UTexture::getMaxAnisotropicLevel();

	freeCamera = std::make_shared<UPerspectiveCamera>("freeCamera");
	freeCamera->setFar(3000.0f);
	freeCamera->setNear(0.1f);
	freeCamera->setModelView(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 400.0f, 990.0f)));

	orthoCamera = std::make_shared<UOrthographicCamera>("orthoCamera");
	orthoCamera->setFar(3000.0f);
	orthoCamera->setNear(0.1f);
	orthoCamera->setModelView(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 400.0f, 990.0f)));

	towerCamera = std::make_shared<UPerspectiveCamera>("towerCamera");
	towerCamera->setFar(3000.0f);
	towerCamera->setNear(0.1f);
	towerCamera->setModelView(glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f)));
	towerCamera->setModelView(glm::rotate(towerCamera->getModelView(), glm::radians(-45.f), glm::vec3(1.f, 0.f, 0.f)));
	towerCamera->setModelView(glm::translate(towerCamera->getModelView(), glm::vec3(-100.f, 100.f, 400.f)));

	fixedCamera = std::make_shared<UPerspectiveCamera>("fixedCamera");
	fixedCamera->setFar(3000.0f);
	fixedCamera->setNear(0.1f);
	fixedCamera->setModelView(glm::translate(glm::mat4(1.0f), glm::vec3(-900.0f, 400.0f, 990.0f)));
	fixedCamera->setModelView(glm::rotate(fixedCamera->getModelView(), glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f)));



	auto root = OVOFactory::getInstance().fromFile("gru28.ovo");

	std::shared_ptr<UMaterial> shadowMaterial = std::make_shared<UMaterial>("shadow", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 128);

	//Node associated to the tower

	std::shared_ptr<UNode> hookNode;
	std::shared_ptr<UNode> towerNode;
	std::shared_ptr<UNode> fisicalHookNode;
	std::shared_ptr<UNode> cableNode;
	std::shared_ptr<UNode> towerCameraNode;


	hookNode = client::ClientUtility::getInstance().findGameObjectByName(root, "hook");
	towerNode = client::ClientUtility::getInstance().findGameObjectByName(root, "tower");
	fisicalHookNode = client::ClientUtility::getInstance().findGameObjectByName(root, "fisicalHook");
	cableNode = client::ClientUtility::getInstance().findGameObjectByName(root, "cable");
	towerCameraNode = client::ClientUtility::getInstance().findGameObjectByName(root, "cameraTower");
	towerCameraNode->addChild(towerCamera);


	//Node associated to all the boxes

	auto hookPoints = client::ClientUtility::getInstance().findGameObjectsByName(root, "hookPoint");
	std::cout << "hook points size: " << hookPoints.size() << std::endl;
	std::vector<std::shared_ptr<client::Box>> boxesVector;

	for (auto& hookPoint : hookPoints)
	{
		auto box = std::make_shared<client::Box>();
		box->setHookPointNode(hookPoint);
		box->setTopNode(client::ClientUtility::getInstance().findGameObjectByName(hookPoint, "top"));
		box->setGroundNode(client::ClientUtility::getInstance().findGameObjectByName(hookPoint, "ground"));
		boxesVector.push_back(box);
	}

	//set node tower

	tower->setTower(towerNode);
	tower->setHook(hookNode);
	tower->setFisicalHook(fisicalHookNode);
	tower->setCable(cableNode);
	tower->setRoot(root);

	tower->setFisicalHookLimitDown(-480.f);
	tower->setFisicalHookLimitUp(-40.f);
	tower->setHookLimitbackward(-335.f);
	tower->setHookLimitforward(0.f);

	//passes boxes to the boxes manager
	boxesManager->setBoxes(boxesVector);


	//2D text creation

	std::shared_ptr<UText> fpsLabel = std::make_shared<UText>("fpsLabel");
	fpsLabel->setColor(glm::vec3(255, 0, 0));

	std::shared_ptr<UText> UpDownLabel = std::make_shared<UText>("UpDownLabel");
	UpDownLabel->setColor(glm::vec3(255, 255, 255));

	std::shared_ptr<UText> backFrontLabel = std::make_shared<UText>("BackFront");
	backFrontLabel->setColor(glm::vec3(255, 255, 255));

	std::shared_ptr<UText> camerasLabel = std::make_shared<UText>("Camera [1,2,3,4]");
	camerasLabel->setColor(glm::vec3(255, 255, 255));

	std::shared_ptr<UText> rotateTower = std::make_shared<UText>("Tower");
	rotateTower->setColor(glm::vec3(255, 255, 255));

	anisotLevelLabel->setColor(glm::vec3(255, 0, 0));
	textureFilterModeLabel->setColor(glm::vec3(255, 0, 0));

	std::shared_ptr<UText> solidWireFrame = std::make_shared<UText>("solidWireFrame");
	solidWireFrame->setColor(glm::vec3(255, 0, 0));

	std::shared_ptr<UText> cameraMovement = std::make_shared<UText>("cameraMovement");
	cameraMovement->setColor(glm::vec3(255, 255, 255));


	_2DRenderPipeline->pass(fpsLabel, glm::vec2(10, 10));
	_2DRenderPipeline->pass(UpDownLabel, glm::vec2(10, 30));
	_2DRenderPipeline->pass(backFrontLabel, glm::vec2(10, 50));
	_2DRenderPipeline->pass(rotateTower, glm::vec2(10, 70));
	_2DRenderPipeline->pass(camerasLabel, glm::vec2(10, 90));
	_2DRenderPipeline->pass(cameraMovement, glm::vec2(10, 110));
	_2DRenderPipeline->pass(anisotLevelLabel, glm::vec2(10, 130));
	_2DRenderPipeline->pass(textureFilterModeLabel, glm::vec2(10, 150));
	_2DRenderPipeline->pass(solidWireFrame, glm::vec2(10, 170));

	UpDownLabel->setText("[+/-] UpDown Hook");
	backFrontLabel->setText("[ARROW UP/DOWN] FrontBack Hook");
	rotateTower->setText("[ARROW LEFT/RIGHT] Rotate Tower");
	camerasLabel->setText("[1,2,3,4] Change Camera View");
	solidWireFrame->setText("[z/x] Solid/Wireframe mode");
	cameraMovement->setText("[WASD R/F] R/F=UP/DOWN");


	UCamera::setMainCamera(fixedCamera);

	Utopia::getInstance().enableTexturesRepeat();
	Utopia::getInstance().enableLinearBipmapLinearFilter();
	Utopia::getInstance().updateAnisotropyLevelAllTextures(UTexture::getMaxAnisotropicLevel());
	anisotLevelLabel->setText("[c] AnisotropicLevel: " + std::to_string(UTexture::getMaxAnisotropicLevel()));
	textureFilterModeLabel->setText("[v] textureFilterMode: LinearBipmapLinear");


	while (Utopia::getInstance().isRunning())
	{
		Utopia::getInstance().mainLoop();
		Utopia::getInstance().clear();

		boxesManager->computeGravity();
		fpsCounter++;
		fpsLabel->setText(std::to_string(fpsToPrint));

		shadowRenderPipeline->clear();
		shadowRenderPipeline->pass(towerNode, glm::scale(glm::translate(glm::mat4(1), glm::vec3(0.f, 1.0f, .0f)), glm::vec3(1.f, 0.f, 1.f)), shadowMaterial);
		for (auto& hook : hookPoints)
		{
			shadowRenderPipeline->pass(hook, glm::translate(glm::mat4(1.f), glm::vec3(0.f,1.0f,0.f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 0.f, 1.f)), shadowMaterial);
		}
		shadowRenderPipeline->render();

		_3DRenderPipeline->clear();
		_3DRenderPipeline->pass(root);
		_3DRenderPipeline->render();


		_2DRenderPipeline->render();
		Utopia::getInstance().enableLighting();
		Utopia::getInstance().enableShadeModel();
		Utopia::getInstance().swap();
	}

	std::cout << "Terminate" << std::endl;
}

void passiveMotionCallback(int x, int y)
{

	if (UCamera::getMainCamera().lock()->getName() != "freeCamera")
		return;

	static int previousPositionX = x;
	static int previousPositionY = y;

	static float angleX = 0;
	static float angleY = 0;

	float xRotationToDo = (float)(previousPositionX - x);
	float yRotationToDo = (float)(previousPositionY - y);

	angleX += xRotationToDo * sensibility;
	angleY += yRotationToDo * sensibility;

	if (angleY > 80.f)
	{
		angleY = 80.f;
	}

	if (angleY < -80.f)
	{
		angleY = -80.f;
	}


	previousPositionX = x;
	previousPositionY = y;

	glm::vec3 cameraLocalPosition = client::ClientUtility::getInstance().getLocalPosition(freeCamera);
	glm::mat4 newCameraMatrix = glm::translate(glm::mat4(1.f), cameraLocalPosition);

	glm::mat4 matRotationX = glm::rotate(newCameraMatrix, glm::radians(angleX), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 matRotationY = glm::rotate(matRotationX, glm::radians(angleY), glm::vec3(1.f, 0.f, 0.f));

	freeCamera->setModelView(matRotationY);

	std::cout << "angleX: " << angleX << std::endl;
	std::cout << "angleY: " << angleY << std::endl;
}


void keyboardCallback(unsigned char key, int mouseX, int mouseY)
{
	static int currentAniLevel = 0;
	static int currentTexturesVisualization = 0;

	glm::vec3 cameraNewPos = glm::vec4(0, 0, 0, 1);


	auto box = boxesManager->possibleBoxToHook(tower->getFisicalHook(), 150);


	switch (key)
	{

	case 'a':
		cameraNewPos.x -= 10.00f;
		break;
	case 'd':
		cameraNewPos.x += 10.00f;
		break;
	case 's':
		cameraNewPos.z += 10.00f;
		break;
	case 'w':
		cameraNewPos.z -= 10.00f;
		break;
	case 'r':
		cameraNewPos.y += 10.00f;
		break;
	case 'f':
		cameraNewPos.y -= 10.00f;
		break;

	case 'z':
		Utopia::getInstance().enableSolidMode();
		break;

	case 'x':
		Utopia::getInstance().enableWireFrameMode();
		break;

	case 'c':
		Utopia::getInstance().updateAnisotropyLevelAllTextures(++currentAniLevel);
		anisotLevelLabel->setText("[c] AnisotropicLevel: " + std::to_string(currentAniLevel));
		if (currentAniLevel >= UTexture::getMaxAnisotropicLevel())
			currentAniLevel = 0;
		break;

	case 'v':

		switch (currentTexturesVisualization++)
		{
		case 0:
			Utopia::getInstance().enableNearestFilter();
			textureFilterModeLabel->setText("[v] textureFilterMode: Nearest");
			break;
		case 1:
			Utopia::getInstance().enableNearestBipmapNearestFilter();
			textureFilterModeLabel->setText("[v] textureFilterMode: NearestBipmapNearest");
			break;
		case 2:
			Utopia::getInstance().enableLinearFilter();
			textureFilterModeLabel->setText("[v] textureFilterMode: Linear");
			break;
		case 3:
			Utopia::getInstance().enableLinearBipmapNearestFilter();
			textureFilterModeLabel->setText("[v] textureFilterMode: LinearBipmapNearest");
			break;
		case 4:
			Utopia::getInstance().enableLinearBipmapLinearFilter();
			textureFilterModeLabel->setText("[v] textureFilterMode: LinearBipmapLinear");

			break;
		}
		if (currentTexturesVisualization > 4)
			currentTexturesVisualization = 0;




	case '1':
		UCamera::setMainCamera(freeCamera);
		break;
	case '2':
		UCamera::setMainCamera(towerCamera);
		break;
	case '3':
		UCamera::setMainCamera(orthoCamera);
		break;
	case '4':
		UCamera::setMainCamera(fixedCamera);
		break;


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
		tower->moveFisicalHookUpDown(-6.f);
		break;
	case '+':
		tower->moveFisicalHookUpDown(+6.f);
		break;
	}

	if (UCamera::getMainCamera().lock()->getName() == "freeCamera")
	{
		freeCamera->setModelView(glm::translate(freeCamera->getModelView(), cameraNewPos));
		std::cout << "camera" << std::endl;
		std::cout << glm::to_string(client::ClientUtility::getInstance().getLocalPosition(freeCamera)) << std::endl;
	}

}

void fpsCounterCallback(int value)
{
	fpsToPrint = fpsCounter;
	fpsCounter = 0;
	Utopia::getInstance().setTimer(1000, fpsCounterCallback, 0);
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
		tower->moveHookBackwardForward(15.f);
		break;

	case (int)USpecialKeys::KEY_DOWN:
		tower->moveHookBackwardForward(-15.f);
		break;

	}
}

void closeCallback()
{
	std::cout << "Close" << std::endl;
	Utopia::getInstance().free();
}
