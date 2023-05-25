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
#include "UTextureFactory.h"
#include "UHands.h"

#include "extensions.h"
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
std::shared_ptr<UNode> towerCameraNode;


std::unique_ptr<client::Tower> tower = std::unique_ptr<client::Tower>(new client::Tower());
std::unique_ptr <client::BoxesManager> boxesManager = std::unique_ptr<client::BoxesManager>(new  client::BoxesManager());

float sensibility = 0.5f;

std::shared_ptr<UHands> hands;
void handsUpdateCallback(int value);
//int handsUpdateTime = 1; //milliseconds

//buttons
struct CraneButton {
	std::shared_ptr<UMesh> m_button;
	bool m_isActive;
	std::shared_ptr<UMaterial> m_oldMaterial;
	void (*m_handler)();
	CraneButton(std::shared_ptr<UMesh> button, bool isActive, std::shared_ptr<UMaterial> oldMaterial) : m_button{ button }, m_isActive{ isActive }, m_oldMaterial{ oldMaterial }, m_handler{ [](){} } {}
};
std::map<std::string, std::shared_ptr<CraneButton>> buttons;
std::shared_ptr<UMaterial> activeMaterial;
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

	auto _3DRenderPipeline = std::unique_ptr<U3DRenderPipeline>(new U3DRenderPipeline("3DRenderPipeline"));

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

	std::cout << "hands init" << std::endl;
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


	hookNode = client::extensions::findGameObjectByName(root, "hook");
	towerNode = client::extensions::findGameObjectByName(root, "tower");
	fisicalHookNode = client::extensions::findGameObjectByName(root, "fisicalHook");
	cableNode = client::extensions::findGameObjectByName(root, "cable");

	/*
	towerCameraNode = client::ClientUtility::getInstance().findGameObjectByName(root, "cameraTowerNoStereo");

	if (!Utopia::getInstance().isStereoscopicEnabled()) //if the openvr mode is enabled under conf/global.conf the camera will be internally setted by the engine
	{
		towerCameraNode->addChild(towerCamera);
		UCamera::setMainCamera(fixedCamera);
	}
	else
	{
		hands->setXDistanceFromCam(0.0f);
		hands->setYDistanceFromCam(-0.37f);
		hands->setZDistanceFromCam(0.4f);
		towerCameraNode->setModelView(glm::rotate(towerCameraNode->getModelView(), glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f)));
		UCamera::getMainCamera().lock()->setFar(4000.0f);
		UCamera::getMainCamera().lock()->setNear(0.1f);
		towerCameraNode->addChild(UCamera::getMainCamera().lock()); //in openvr mode we fix the camera position into the cabin
	}
	*/

	if (!Utopia::getInstance().isStereoscopicEnabled()) //if the openvr mode is enabled under conf/global.conf the camera will be internally setted by the engine
	{
		towerCameraNode = client::extensions::findGameObjectByName(root, "cameraTowerNoStereo");
		towerCameraNode->addChild(towerCamera);
		UCamera::setMainCamera(fixedCamera);
	}
	else
	{
		hands->setXDistanceFromCam(0.0f);
		hands->setYDistanceFromCam(-0.37f);
		hands->setZDistanceFromCam(0.4f);
		towerCameraNode = client::extensions::findGameObjectByName(root, "cameraTower");
		towerCameraNode->setModelView(glm::rotate(towerCameraNode->getModelView(), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f))); //the point as defined in 3ds is wrongly rotated of -90 degrees. We cancel back the rotation.
		UCamera::getMainCamera().lock()->setFar(4000.0f);
		UCamera::getMainCamera().lock()->setNear(0.1f);
		towerCameraNode->addChild(UCamera::getMainCamera().lock()); //in openvr mode we fix the camera position into the cabin
	}


	//Node associated to all the boxes

	auto hookPoints = client::extensions::findGameObjectsByName(root, "hookPoint");
	std::cout << "hook points size: " << hookPoints.size() << std::endl;
	std::vector<std::shared_ptr<client::Box>> boxesVector;

	for (auto& hookPoint : hookPoints)
	{
		auto box = std::make_shared<client::Box>();
		box->setHookPointNode(hookPoint);
		box->setTopNode(client::extensions::findGameObjectByName(hookPoint, "top"));
		box->setGroundNode(client::extensions::findGameObjectByName(hookPoint, "ground"));
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

	std::cout << "Lights used: " << ULight::getnLightsUsed() << std::endl;
	std::cout << "Starting main loop" << std::endl;

	Utopia::getInstance().setTimer(simulationDelay, boxesSimulationCounterCallback, 0);
	
	//buttons 
	auto controls = client::extensions::findGameObjectByName(root, "controls");
	controls->setModelView(glm::translate(controls->getModelView(), glm::vec3(-0.4f, 0.2f,0.0f)));

	buttons = std::map<std::string, std::shared_ptr<CraneButton>>();
	for (auto name : buttonsNames)
	{
		auto b = std::dynamic_pointer_cast<UMesh>(client::extensions::findGameObjectByName(controls, name));
		buttons[name] = std::make_shared<CraneButton>(b, false, b->getMaterial());
	}
	buttons["cart_backward"]->m_handler = []() {tower->moveHookBackwardForward(-0.3f);};
	buttons["cart_forward"]->m_handler = []() {tower->moveHookBackwardForward(0.3f); };
	buttons["crane_down"]->m_handler = []() {tower->moveFisicalHookUpDown(-0.3f); };
	buttons["crane_up"]->m_handler = []() {tower->moveFisicalHookUpDown(+0.3f); };
	buttons["left"]->m_handler = []() {tower->rotateTower(glm::radians(0.5f)); };
	buttons["right"]->m_handler = []() {tower->rotateTower(glm::radians(-0.5f)); };
	buttons["pick"]->m_handler = []() {	
		auto box = boxesManager->possibleBoxToHook(tower->getFisicalHook(), 3);
		auto now = std::chrono::high_resolution_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - hookLastUpdate).count() >= hookDelay) {
			if (box != nullptr && !tower->isHooking())
				tower->take(box);
			else if (tower->isHooking())
					tower->release();
			hookLastUpdate = now;
		}
	};
	activeMaterial = std::make_shared<UMaterial>("activeMat", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 128);
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

	glm::vec3 cameraLocalPosition = client::extensions::getLocalPosition(currentCamera);
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

	glm::vec3 towerCameraNodeNewPos = glm::vec4(0, 0, 0, 1);

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
		if (currentAniLevel >= maxAnisotropyLevel)
			currentAniLevel = 0;
		break;

	case 'v':
		switch (currentTexturesVisualization++)
		{
		case 0:
			U2DTexture::forEach([](std::shared_ptr<U2DTexture> texture) { texture->enableNearestFilter(); });
			break;
		case 1:
			U2DTexture::forEach([](std::shared_ptr<U2DTexture> texture) { texture->enableNearestBitmapNearestFilter(); });
			break;
		case 2:
			U2DTexture::forEach([](std::shared_ptr<U2DTexture> texture) { texture->enableLinearFilter(); });
			break;
		case 3:
			U2DTexture::forEach([](std::shared_ptr<U2DTexture> texture) { texture->enableLinearBitmapNearestFilter(); });
			break;
		case 4:
			U2DTexture::forEach([](std::shared_ptr<U2DTexture> texture) { texture->enableLinearBitmapLinearFilter(); });
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
#ifdef _DEBUG
				std::cout << "is Hooking: " << std::endl;
#endif // _DEBUG
			}
#ifdef _DEBUG
			else
			{
				std::cout << "is not Hooking: " << std::endl;
			}
#endif // _DEBUG
		}

		break;


	case '-':
		tower->moveFisicalHookUpDown(-0.3f);
		break;
	case '+':
		tower->moveFisicalHookUpDown(+0.3f);
		break;

	case 'n':
		towerCameraNodeNewPos.x -= 0.025f;
		break;

	case 'u':
		towerCameraNodeNewPos.y += 0.025f;

		break;


	case 'j':
		towerCameraNodeNewPos.y -= 0.025f;
		break;


	case 'm':
		towerCameraNodeNewPos.x += 0.025f;
		break;

	case 'h':
		towerCameraNodeNewPos.z -= 0.025f;
		break;

	case 'k':
		towerCameraNodeNewPos.z += 0.025f;
		break;

	}

	if (UCamera::getMainCamera().lock()->getName() == "freeCamera")
	{
		freeCamera->setModelView(glm::translate(freeCamera->getModelView(), cameraNewPos));
#ifdef _DEBUG
		std::cout << "camera" << std::endl;
		std::cout << glm::to_string(client::extensions::getLocalPosition(freeCamera)) << std::endl;
#endif // _DEBUG
	}

	towerCameraNode->setModelView(glm::translate(towerCameraNode->getModelView(), towerCameraNodeNewPos));
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

void handsCollisionCallback(int value) {
	bool in;
	std::shared_ptr<CraneButton> craneButton;
	for (auto name : buttonsNames) {
		craneButton = buttons[name];
		in = hands->checkIfHandsAreIn(craneButton->m_button);
		if (craneButton->m_isActive && !in) {
			craneButton->m_button->setMaterial(craneButton->m_oldMaterial);
		}
		else if (!craneButton->m_isActive && in) {
			craneButton->m_button->setMaterial(activeMaterial);
		}
		craneButton->m_isActive = in;
		if (in)
			craneButton->m_handler();
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
