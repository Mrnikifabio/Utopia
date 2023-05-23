/*****************************************************************//**
 * \file   utopia.cpp
 * \brief  Implementation of utopia.h
 *
 * \author Group 10
 * \date   October 2022
 *********************************************************************/

#include <string.h>
#include <iostream>
#include <fstream>
#include "Utopia.h"
#include "UCamera.h"
#include <GL/glew.h>
#include <gl/freeglut.h>
#include <unordered_map>
#include "UTexture.h"
#include <memory>
#include "ULight.h"
#include "UVertexShader.h"
#include "UFragmentShader.h"
#include "UProgramShader.h"
#include "UOVRCamera.h"
#include <UFbo.h>
#include "U2DTexture.h"
#include "UResources.h"



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


 //////////////
 // DLL MAIN //
 //////////////

#ifdef _WINDOWS
#include <Windows.h>
#include <resource.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * DLL entry point. Avoid to rely on it for easier code portability (Linux doesn't use this method).
 * @param instDLL handle
 * @param reason reason
 * @param _reserved reserved
 * @return true on success, false on failure
 */
int APIENTRY DllMain(HANDLE instDLL, DWORD reason, LPVOID _reserved)
{
	// Check use:
	switch (reason)
	{
		///////////////////////////
	case DLL_PROCESS_ATTACH: //
		break;


		///////////////////////////
	case DLL_PROCESS_DETACH: //
		break;
	}

	// Done:
	return true;
}
#endif

using namespace utopia;

struct Utopia::pimpl
{
	bool m_initFlag;
	bool m_steroscopicRender;

	std::shared_ptr<UFragmentShader> m_basicFragShader;
	std::shared_ptr<UVertexShader> m_basicVertShader;
	std::shared_ptr<UProgramShader> m_basicProgShader;
	std::shared_ptr<UFragmentShader> m_passThroughFragShader;
	std::shared_ptr<UVertexShader> m_passThroughVertShader;
	std::shared_ptr<UProgramShader> m_passThroughProgShader;

	std::shared_ptr<OvVR> m_uvr;
	std::shared_ptr<UOVRCamera> m_ovrCamera;

	pimpl() :
		m_initFlag{ false }, 
		m_steroscopicRender{ false },
		m_uvr{ std::shared_ptr<OvVR>(new OvVR("OpenVR wrapper")) },
		m_basicFragShader{std::shared_ptr<UFragmentShader>(new UFragmentShader("basicFrag")) },
		m_basicVertShader{std::shared_ptr<UVertexShader>(new UVertexShader("basicVert"))},
		m_basicProgShader{std::shared_ptr<UProgramShader>(new UProgramShader("basicProgShader"))},
		m_passThroughFragShader{ std::shared_ptr<UFragmentShader>(new UFragmentShader("passThroughFragShader")) },
		m_passThroughVertShader{ std::shared_ptr<UVertexShader>(new UVertexShader("passThrougVertShader")) },
		m_passThroughProgShader{ std::shared_ptr<UProgramShader>(new UProgramShader("passThrougProgShader")) }
	{}
};

Utopia::Utopia() : m_pimpl{ std::unique_ptr<Utopia::pimpl>(new pimpl()) } {};
Utopia::~Utopia() {}

std::shared_ptr<UProgramShader> Utopia::getBasicProgramShader()
{
	return m_pimpl->m_basicProgShader;
}


void displayCallback()
{
}


void reshapeCallback(int width, int height)
{
	auto projLoc = UProgramShader::getActiveProgramShader()->getParamLocation("projection");
	UProgramShader::getActiveProgramShader()->setMatrix4(projLoc, UCamera::getMainCamera().lock()->getCameraMatrix());

	// (bad) trick to avoid window resizing:
	if (width != APP_WINDOWSIZEX || height != APP_WINDOWSIZEY)
		glutReshapeWindow(APP_WINDOWSIZEX, APP_WINDOWSIZEY);
	
#ifdef _DEBUG
	std::cout << "[reshape func invoked] " << width << " " << height << std::endl;
#endif // DEBUG
}

#ifdef _DEBUG
void __stdcall DebugCallback(GLenum source, GLenum type,
	GLuint id, GLenum severity,
	GLsizei length,
	const GLchar* message,
	GLvoid* userParam)
{
	printf("OpenGL says: %s\n", message);
}
#endif // DEBUG



bool LIB_API Utopia::init()
{
	bool openVR = false;
	
	//read settings, if more settings will be needed PLEASE! FOR THE LOVE OF GOD PLEASE! Create a class and model the whole thing...
	std::cout<<"Trying reading settings from conf/global.conf..." << std::endl;
	std::ifstream global("conf/global.conf");
	if (global.is_open())
	{
		std::string line;
		while (std::getline(global, line))
		{
			if (line == "openvr=true;") //quite a check uh?
				openVR = true;
		}
		std::cout << "Done" << std::endl;
		global.close();
	}
	else
		std::cout << "Cannot read settings" << std::endl;


	// Prevent double init:
	if (m_pimpl->m_initFlag)
	{
		std::cout << "ERROR: class already initialized" << std::endl;
		return false;
	}

	// Init context:
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitContextVersion(4, 4);
	glutInitContextProfile(GLUT_CORE_PROFILE);
#if _DEBUG
	glutInitContextFlags(GLUT_DEBUG);
#endif
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(APP_WINDOWSIZEX, APP_WINDOWSIZEY);


	char* myargv[1];
	int myargc = 1;
	myargv[0] = { (char*)"Utopia" };
	glutInit(&myargc, myargv);

	// Set some optional flags:
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_EXIT);

	// Create the window with a specific title:
	glutCreateWindow("Utopia Window");

	// Init Glew (*after* the context creation):
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		// Error loading GLEW
		std::cout << "ERROR: failed to loading GLEW" << std::endl;
		return false;
	}


	// OpenGL 4.4 is required:
	if (!glewIsSupported("GL_VERSION_4_4"))
	{
		std::cout << "OpenGL 4.4 not supported" << std::endl;
		return false;
	}
	
	if (openVR)
	{
		//in openVR mode the camera is fixed and controlled in U3DRenderPipeline through the OVR Wrapper
		m_pimpl->m_ovrCamera = std::shared_ptr<UOVRCamera>(new UOVRCamera("openVRCamera"));
		UCamera::setMainCamera(m_pimpl->m_ovrCamera);

		if (m_pimpl->m_uvr->init() == false)
		{
			std::cout << "[ERROR] Unable to init OpenVR" << std::endl;
			return false;
		}
		enableStereoscopic(true);
	}

	// The OpenGL context is now initialized...
	setDisplayCallback(displayCallback);
	setReshapeCallback(reshapeCallback);

	U2DTexture::forEach([](std::shared_ptr<U2DTexture> texture) {
		texture->enableTextureClampToEdge();
		texture->enableLinearFilter();
	});

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);

#if _DEBUG
	// Register OpenGL debug callback:
	glDebugMessageCallback((GLDEBUGPROC)DebugCallback, nullptr);
	// Enable debug notifications:
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif


	// Check OpenGL version:
	std::cout << "OpenGL context" << std::endl;
	std::cout << "   version  . . : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "   vendor . . . : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "   renderer . . : " << glGetString(GL_RENDERER) << std::endl;

	m_pimpl->m_basicVertShader->loadFromMemory(UResources::getInstance().getStringResource(IDR_VERT_DEFAULT, VERT));
	m_pimpl->m_basicFragShader->loadFromMemory(UResources::getInstance().getStringResource(IDR_FRAG_DEFAULT, FRAG));

	UProgramShader::setActiveProgramShader(m_pimpl->m_basicProgShader);
	m_pimpl->m_basicProgShader->build(*m_pimpl->m_basicVertShader, *m_pimpl->m_basicFragShader);
	m_pimpl->m_basicProgShader->render();
	m_pimpl->m_basicProgShader->bind(0, "in_Position");
	m_pimpl->m_basicProgShader->bind(1, "in_Normal");

	m_pimpl->m_passThroughVertShader->loadFromMemory(UResources::getInstance().getStringResource(IDR_VERT_PASS_THROUGH, VERT));
	m_pimpl->m_passThroughFragShader->loadFromMemory(UResources::getInstance().getStringResource(IDR_FRAG_PASS_THROUGH, FRAG));
	m_pimpl->m_passThroughProgShader->build(*m_pimpl->m_passThroughVertShader, *m_pimpl->m_passThroughFragShader);

	// Done:
	m_pimpl->m_initFlag = true;

	return true;
}

void Utopia::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Deinitialization method.
 * @return true on success, false on error
 */
void LIB_API Utopia::free()
{
	// Really necessary?
	if (!m_pimpl.get())
	{
		std::cout << "ERROR: class not initialized" << std::endl;
	}
	else
		m_pimpl->m_uvr->free();
	
	// Done:
	m_pimpl->m_initFlag = false;
}

void Utopia::enableDepth()
{
	glEnable(GL_DEPTH_TEST);
}

void Utopia::enableCullFace()
{
	glEnable(GL_CULL_FACE);
}

void Utopia::enableLighting()
{
	glEnable(GL_LIGHTING);
}

void Utopia::enableShadeModel() {
	glEnable(GL_SMOOTH);
}

void Utopia::enableWireFrameMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Utopia::enableSolidMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Utopia::setKeyboardCallback(void(*callback)(unsigned char, int, int))
{
	glutKeyboardFunc(callback);
}

void Utopia::setDisplayCallback(void(*callback)(void))
{
	glutDisplayFunc(callback);
}

void Utopia::setReshapeCallback(void(*callback)(int, int))
{
	glutReshapeFunc(callback);
}

void Utopia::setSpecialCallback(void(*callback)(int, int, int))
{
	glutSpecialFunc(callback);
}

void Utopia::setCloseCallback(void(*callback)(void))
{
	glutCloseFunc(callback);
}

void utopia::Utopia::setPassiveMotionCallback(void(*callback)(int, int))
{
	glutPassiveMotionFunc(callback);
}

void Utopia::mainLoop()
{
	glutMainLoopEvent();
}

bool Utopia::isRunning()
{
	return true;
}

void Utopia::swap()
{
	glutSwapBuffers();
}

LIB_API void utopia::Utopia::setBackgroundColor(glm::vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

int Utopia::getWindowWidth()
{
	if (Utopia::isStereoscopicEnabled())
		return glutGet(GLUT_WINDOW_WIDTH) / 2;
	else
		return glutGet(GLUT_WINDOW_WIDTH);
}

int Utopia::getWindowHeight()
{
	return glutGet(GLUT_WINDOW_HEIGHT);
}

void Utopia::setTimer(int timeoutMs, void(*callback)(int), int value)
{
	glutTimerFunc(timeoutMs, callback, value);
}


std::shared_ptr<UVertexShader> utopia::Utopia::getBasicVertexShader() {
	return m_pimpl->m_basicVertShader;
}

std::shared_ptr<UFragmentShader> utopia::Utopia::getBasicFragmentShader() {
	return m_pimpl->m_basicFragShader;
}

std::shared_ptr<UVertexShader> utopia::Utopia::getPassThroughVertShader() {
	return m_pimpl->m_passThroughVertShader;
}
std::shared_ptr<UFragmentShader> utopia::Utopia::getPassThroughFragmentShader() {
	return m_pimpl->m_passThroughFragShader;
}

std::shared_ptr<UProgramShader> utopia::Utopia::getPassThroughProgamShader() {
	return m_pimpl->m_passThroughProgShader;
}

std::shared_ptr<OvVR> utopia::Utopia::getOpenVRWrapper()
{
	return m_pimpl->m_uvr;
}

Utopia& Utopia::getInstance()
{
	static Utopia m_instance; // Guaranteed to be destroyed.
	// Instantiated on first use.
	return m_instance;
}

void utopia::Utopia::enableStereoscopic(const bool enable)
{
	m_pimpl->m_steroscopicRender = enable;
}

bool utopia::Utopia::isStereoscopicEnabled() {
	return m_pimpl->m_steroscopicRender;
}
