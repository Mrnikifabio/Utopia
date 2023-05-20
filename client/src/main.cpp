#include <iostream>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <map>
#include <chrono>

#include "Utopia.h"
#include "UShader.h"
#include "UNode.h"
#include "UMesh.h"
#include "UPerspectiveCamera.h"
#include "UOrthographicCamera.h"
#include "OVOFactory.h"
#include "UOmniLight.h"
#include "USpecialKeys.h"
#include "U2DTexture.h"
#include "UText.h"
#include "UTextureFactory.h"
#include "UHands.h"

#include "ClientUtility.h"
#include "Tower.h"
#include "BoxesManager.h"
#include "Box.h"



using namespace utopia;

void keyboardCallback(unsigned char key, int mouseX, int mouseY);
void specialCallback(int key, int mouseX, int mouseY);
void passiveMotionCallback(int x, int y);
void boxesSimulationCounterCallback(int value);
void closeCallback();
void handsCollisionCallback(int value);

int maxAnisotropyLevel;

const int simulationDelay = 32; //milliseconds

std::shared_ptr<UCamera> freeCamera;
std::shared_ptr<UCamera> fixedCamera;
std::shared_ptr<UCamera> towerCamera;


std::unique_ptr<client::Tower> tower = std::unique_ptr<client::Tower>(new client::Tower());
std::unique_ptr <client::BoxesManager> boxesManager = std::unique_ptr<client::BoxesManager>(new  client::BoxesManager());

float sensibility = 0.5f;

std::shared_ptr<UText> anisotLevelLabel = std::make_shared<UText>("anisotLevelLabel");
std::shared_ptr<UText> textureFilterModeLabel = std::make_shared<UText>("textureFilterMode");

std::shared_ptr<UHands> hands;
void handsUpdateCallback(int value);
//int handsUpdateTime = 1; //milliseconds

std::map<std::string, std::shared_ptr<UNode>> buttons;
std::vector<std::string> buttonsNames = { "cart_backward", "cart_forward", "crane_down", "crane_up", "left", "pick", "right" };
int buttonUpdate = 50; //milliseconds
int hookDelay = 1000; //milliseconds
std::chrono::steady_clock::time_point hookLastUpdate = std::chrono::high_resolution_clock::now();

int main()
{
	if (!Utopia::getInstance().init())
		return 0;
	Utopia::getInstance().setBackgroundColor(glm::vec4(1.0f, 1.f, 1.f, 1.f));
	Utopia::getInstance().enableDepth();
	Utopia::getInstance().enableCullFace();
	Utopia::getInstance().setKeyboardCallback(keyboardCallback);
	Utopia::getInstance().setSpecialCallback(specialCallback);
	Utopia::getInstance().setCloseCallback(closeCallback);
	Utopia::getInstance().setPassiveMotionCallback(passiveMotionCallback);

	auto _2DRenderPipeline = std::unique_ptr<U2DRenderPipeline>(new U2DRenderPipeline("2DRenderPipeline"));
	auto _3DRenderPipeline = std::unique_ptr<U3DRenderPipeline>(new U3DRenderPipeline("3DRenderPipeline"));
	auto shadowRenderPipeline = std::unique_ptr<U3DRenderPipeline>(new U3DRenderPipeline("shadowRenderPipeline"));

	maxAnisotropyLevel = UTexture::getMaxAnisotropicLevel();

	freeCamera = std::make_shared<UPerspectiveCamera>("freeCamera");
	freeCamera->setFar(4000.0f);
	freeCamera->setNear(0.1f);
	freeCamera->setModelView(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.8f, 0.0f)));

	towerCamera = std::make_shared<UPerspectiveCamera>("towerCamera");
	towerCamera->setFar(4000.0f);
	towerCamera->setNear(0.1f);
	towerCamera->setModelView(glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f)));
	towerCamera->setModelView(glm::rotate(towerCamera->getModelView(), glm::radians(-45.f), glm::vec3(1.f, 0.f, 0.f)));

	fixedCamera = std::make_shared<UPerspectiveCamera>("fixedCamera");
	fixedCamera->setFar(4000.0f);
	fixedCamera->setNear(0.1f);
	fixedCamera->setModelView(glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 30.0f, 50.0f)));
	fixedCamera->setModelView(glm::rotate(fixedCamera->getModelView(), glm::radians(-40.f), glm::vec3(0.f, 1.f, 0.f)));
	fixedCamera->setModelView(glm::rotate(fixedCamera->getModelView(), glm::radians(-22.f), glm::vec3(1.f, 0.f, 0.f)));

	auto root = OVOFactory::getInstance().fromFile("gru28.ovo");
	
	hands = std::make_shared<UHands>();
	root->addChild(hands);

	if (!hands->init())
		std::cout << "error with hands maybe leap not connected/not working or hands model not present" << std::endl;
	//else
		//Utopia::getInstance().setTimer(handsUpdateTime, handsUpdateCallback, handsUpdateTime);

	auto cubeMap = UTextureFactory::getInstance().fromFileCubeMaps({
	  "skybox/px.png",
	  "skybox/nx.png",
	  "skybox/py.png",
	  "skybox/ny.png",
	  "skybox/pz.png",
	  "skybox/nz.png",
	});

	cubeMap->enableTextureClampToEdge();
	_3DRenderPipeline->enableSkybox(cubeMap, glm::scale(glm::mat4(1.0f), glm::vec3(550.0f, 550.0f, 550.0f)));

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

	if (!Utopia::getInstance().isStereoscopicEnabled()) //if the openvr mode is enabled under conf/global.conf the camera will be internally setted by the engine
	{
		towerCameraNode = client::ClientUtility::getInstance().findGameObjectByName(root, "cameraTowerNoStereo");
		towerCameraNode->addChild(towerCamera);
		UCamera::setMainCamera(fixedCamera);
	}
	else
	{
		UCamera::getMainCamera().lock()->setFar(4000.0f);
		UCamera::getMainCamera().lock()->setNear(0.1f);
		towerCameraNode->addChild(UCamera::getMainCamera().lock()); //in openvr mode we fix the camera position into the cabin
	}

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

	tower->setFisicalHookLimitDown(-24.f);
	tower->setFisicalHookLimitUp(-1.f);
	tower->setHookLimitbackward(-12.0f);
	tower->setHookLimitforward(3.5f);

	//passes boxes to the boxes manager
	boxesManager->setBoxes(boxesVector);

	U2DTexture::forEach([](std::shared_ptr<U2DTexture> texture) {
		texture->enableTextureRepeat();
		texture->enableLinearBitmapLinearFilter();
		texture->updateAnisotropyLevel(maxAnisotropyLevel);
	});

	anisotLevelLabel->setText("[c] AnisotropicLevel: " + std::to_string(maxAnisotropyLevel));
	textureFilterModeLabel->setText("[v] textureFilterMode: LinearBitmapLinear");

	std::cout << "Lights used: " << ULight::getnLightsUsed() << std::endl;
	std::cout << "Starting main loop" << std::endl;

	Utopia::getInstance().setTimer(simulationDelay, boxesSimulationCounterCallback, 0);
	
	auto controls = client::ClientUtility::getInstance().findGameObjectByName(root, "controls");
	buttons = std::map<std::string, std::shared_ptr<UNode>>();
	for (auto name : buttonsNames)
	{
		buttons[name] = client::ClientUtility::getInstance().findGameObjectByName(controls, name);
	}
	Utopia::getInstance().setTimer(buttonUpdate, handsCollisionCallback, buttonUpdate);
	while (Utopia::getInstance().isRunning())
	{
		Utopia::getInstance().mainLoop();
		Utopia::getInstance().clear();

		hands->update();
		_3DRenderPipeline->clear();
		_3DRenderPipeline->pass(root);
		_3DRenderPipeline->render();

		Utopia::getInstance().swap();
	}

	std::cout << "Terminate" << std::endl;
}

void passiveMotionCallback(int x, int y)
{
	auto currentCamera = UCamera::getMainCamera().lock();
	auto cameraName = currentCamera->getName();

	if (cameraName != "freeCamera" && cameraName != "towerCamera")
		return;

	static int previousPositionX = x;
	static int previousPositionY = y;

	static float angleX = -90.0f;
	static float angleY = -45.0f;

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

	glm::vec3 cameraLocalPosition = client::ClientUtility::getInstance().getLocalPosition(currentCamera);
	glm::mat4 newCameraMatrix = glm::translate(glm::mat4(1.f), cameraLocalPosition);

	glm::mat4 matRotationX = glm::rotate(newCameraMatrix, glm::radians(angleX), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 matRotationY = glm::rotate(matRotationX, glm::radians(angleY), glm::vec3(1.f, 0.f, 0.f));

	currentCamera->setModelView(matRotationY);
}

void changeCamera(std::weak_ptr<UCamera> camera)
{
	if(!Utopia::getInstance().isStereoscopicEnabled())
		UCamera::setMainCamera(camera);
}

void keyboardCallback(unsigned char key, int mouseX, int mouseY)
{
	static int currentAniLevel = 0;
	static int currentTexturesVisualization = 0;

	glm::vec3 cameraNewPos = glm::vec4(0, 0, 0, 1);
	glm::vec3 lightNewPos = glm::vec4(0, 0, 0, 1);

	bool isLightMoved=false;


	auto box = boxesManager->possibleBoxToHook(tower->getFisicalHook(), 3);

	switch (key)
	{


	case 'a': case 'A':
		cameraNewPos.x -= 1.00f;
		break;
	case 'd': case 'D':
		cameraNewPos.x += 1.00f;
		break;
	case 's': case 'S':
		cameraNewPos.z += 1.00f;
		break;
	case 'w':
		cameraNewPos.z -= 1.00f;
		break;
	case 'r':
		cameraNewPos.y += 1.00f;
		break;
	case 'f':
		cameraNewPos.y -= 1.00f;
		break;

	case 'z':
		Utopia::getInstance().enableSolidMode();
		break;

	case 'x':
		Utopia::getInstance().enableWireFrameMode();
		break;

	case 'c':
		U2DTexture::forEach([](std::shared_ptr<U2DTexture> texture) { texture->updateAnisotropyLevel(++currentAniLevel); });
		anisotLevelLabel->setText("[c] AnisotropicLevel: " + std::to_string(currentAniLevel));
		if (currentAniLevel >= maxAnisotropyLevel)
			currentAniLevel = 0;
		break;

	case 'v':
		switch (currentTexturesVisualization++)
		{
		case 0:
			U2DTexture::forEach([](std::shared_ptr<U2DTexture> texture) { texture->enableNearestFilter(); });
			textureFilterModeLabel->setText("[v] textureFilterMode: Nearest");
			break;
		case 1:
			U2DTexture::forEach([](std::shared_ptr<U2DTexture> texture) { texture->enableNearestBitmapNearestFilter(); });
			textureFilterModeLabel->setText("[v] textureFilterMode: NearestBitmapNearest");
			break;
		case 2:
			U2DTexture::forEach([](std::shared_ptr<U2DTexture> texture) { texture->enableLinearFilter(); });
			textureFilterModeLabel->setText("[v] textureFilterMode: Linear");
			break;
		case 3:
			U2DTexture::forEach([](std::shared_ptr<U2DTexture> texture) { texture->enableLinearBitmapNearestFilter(); });
			textureFilterModeLabel->setText("[v] textureFilterMode: LinearBitmapNearest");
			break;
		case 4:
			U2DTexture::forEach([](std::shared_ptr<U2DTexture> texture) { texture->enableLinearBitmapLinearFilter(); });
			textureFilterModeLabel->setText("[v] textureFilterMode: LinearBitmapLinear");
			break;
		}
		if (currentTexturesVisualization > 4)
			currentTexturesVisualization = 0;
		break;



	case '1':
		changeCamera(freeCamera);
		break;
	case '2':
		changeCamera(towerCamera);
		break;
	case '3':
		changeCamera(fixedCamera);
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
		tower->moveFisicalHookUpDown(-0.3f);
		break;
	case '+':
		tower->moveFisicalHookUpDown(+0.3f);
		break;
	}

	if (UCamera::getMainCamera().lock()->getName() == "freeCamera")
	{
		freeCamera->setModelView(glm::translate(freeCamera->getModelView(), cameraNewPos));
		std::cout << "camera" << std::endl;
		std::cout << glm::to_string(client::ClientUtility::getInstance().getLocalPosition(freeCamera)) << std::endl;
	}	

}

void boxesSimulationCounterCallback(int value)
{
	boxesManager->computeGravity();
	Utopia::getInstance().setTimer(simulationDelay, boxesSimulationCounterCallback, 0);
}

void handsUpdateCallback(int value) {
	hands->update();
	Utopia::getInstance().setTimer(value, handsUpdateCallback, value);
}

std::vector<std::string> getButtonNameCollisons() {
	std::vector<std::string> hitted = {};
	for (auto name : buttonsNames) {
		if (hands->checkIfHandsAreIn(std::dynamic_pointer_cast<UMesh>(buttons[name])))
			hitted.push_back(name);
	}
	return hitted;
}

void handsCollisionCallback(int value) {
	for (std::string name : getButtonNameCollisons()) {
		std::cout << name << std::endl;
		auto box = boxesManager->possibleBoxToHook(tower->getFisicalHook(), 3);
		if (name == "cart_backward") {
			tower->moveHookBackwardForward(-0.3f);
		}
		else if (name == "cart_forward") {
			tower->moveHookBackwardForward(0.3f);
		}
		if( name == "crane_down") {
			tower->moveFisicalHookUpDown(-0.3f);
		}
		else if (name == "crane_up") {
			tower->moveFisicalHookUpDown(+0.3f);
		}
		if (name == "left") {
			tower->rotateTower(glm::radians(0.5f));
		}
		else if (name == "right") {
			tower->rotateTower(glm::radians(-0.5f));
		}
		if (name == "pick") {
			auto now = std::chrono::high_resolution_clock::now();
			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - hookLastUpdate).count() >= hookDelay) {
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
				hookLastUpdate = now;
			}
		}

	}
	Utopia::getInstance().setTimer(value, handsCollisionCallback, value);
}

void specialCallback(int key, int mouseX, int mouseY)
{
#if _DEBUG
	std::cout << "[key pressed]" << std::endl;
#endif

	switch (key)
	{

	case (int)USpecialKeys::KEY_LEFT:
		tower->rotateTower(glm::radians(0.5f));
		break;

	case (int)USpecialKeys::KEY_RIGHT:
		tower->rotateTower(glm::radians(-0.5f));
		break;

	case (int)USpecialKeys::KEY_UP:
		tower->moveHookBackwardForward(0.3f);
		break;

	case (int)USpecialKeys::KEY_DOWN:
		tower->moveHookBackwardForward(-0.3f);
		break;

	}
}

void closeCallback()
{
	std::cout << "Close" << std::endl;
	Utopia::getInstance().free();
}
