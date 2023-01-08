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
#include "UText.h"

void keyboardCallback(unsigned char key, int mouseX, int mouseY);
void closeCallback();


using namespace utopia;

float g = -25;
std::shared_ptr<UCamera> camera;


int main()
{
	Utopia::getInstance().init();
	Utopia::getInstance().enableDepth();
	Utopia::getInstance().enableCullFace();
	Utopia::getInstance().setKeyboardCallback(keyboardCallback);
	Utopia::getInstance().setCloseCallback(closeCallback);

	camera = std::make_shared<UPerspectiveCamera>("perspCamera");
	camera->setFar(600.0f);
	camera->setNear(0.1f);
	camera->setModelView(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 45.0f, 320.0f)));

	auto root = OVOFactory::getInstance().fromFile("assets/simple3dScene.ovo");

	auto text = std::make_shared<UText>("objectName", "This is an example of text to show", UText::Font::HELVETICA_18, glm::vec2(1.0f,2.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	UCamera::setMainCamera(camera);
	Utopia::getInstance().get3DRenderPipeline().pass(root, glm::mat4(1));
	Utopia::getInstance().get2DRenderPipeline().pass(text, text->getPosition());

	while (Utopia::getInstance().isRunning())
	{
		Utopia::getInstance().mainLoop();
		Utopia::getInstance().clear();
		Utopia::getInstance().get3DRenderPipeline().render();
		Utopia::getInstance().get2DRenderPipeline().render();
		Utopia::getInstance().enableLighting();
		Utopia::getInstance().enableShadeModel();
		Utopia::getInstance().swap();
	}

	std::cout << "Terminate" << std::endl;
}


void keyboardCallback(unsigned char key, int mouseX, int mouseY)
{
	glm::vec3 cameraNewPos = camera->getModelView() * glm::vec4(0,0,0,1);

	switch (key)
    {

    case 'a':
		cameraNewPos.x -= 1.00f;
		break;
	case 'd':
		cameraNewPos.x += 1.00f;
		break;
	case 's':
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


	}

	camera->setModelView(glm::translate(glm::mat4(1), cameraNewPos));
	std::cout << "camera" << std::endl;
	std::cout << glm::to_string(camera->getModelView()) << std::endl;

}
void closeCallback()
{
	std::cout << "Close" << std::endl;
	Utopia::getInstance().free();
}
