/*****************************************************************//**
 * \file   utopia.cpp
 * \brief  Implementation of utopia.h
 *
 * \author Group 10
 * \date   October 2022
 *********************************************************************/

#include <string.h>
#include <iostream>
#include "Utopia.h"
#include "UCamera.h"
#include <gl/freeglut.h>
#include <unordered_map>
#include "UTexture.h"
#include <memory>
#include "ULight.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//////////////
// DLL MAIN //
//////////////

#ifdef _WINDOWS
#include <Windows.h>
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
	std::unique_ptr<U2DRenderPipeline> m_2DRenderPipeline;
	std::unique_ptr<U3DRenderPipeline> m_3DRenderPipeline;
	std::unordered_map<std::string, std::shared_ptr<UMaterial>> m_materials;
	std::unordered_map<std::string, std::shared_ptr<UTexture>> m_textures;

	pimpl() :
		m_initFlag{ false } {}
};

Utopia::Utopia() : m_pimpl{ std::unique_ptr<Utopia::pimpl>(new pimpl()) } {};
Utopia::~Utopia() {}


void displayCallback()
{

}


void reshapeCallback(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(UCamera::getMainCamera().lock()->getCameraMatrix()));
	glMatrixMode(GL_MODELVIEW);

	std::cout << "[reshape func invoked] " << width<< " " << height << std::endl;
}


bool LIB_API Utopia::init()
{
	// Prevent double init:
	if (m_pimpl->m_initFlag)
	{
		std::cout << "ERROR: class already initialized" << std::endl;
		return false;
	}
	
	// Init context:
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);



	char* myargv[1];
	int myargc = 1;
	myargv[0] = {(char*)"Utopia"};
	glutInit(&myargc, myargv);

	// Set some optional flags:
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_EXIT);

	// Create the window with a specific title:
	glutCreateWindow("Utopia Window");

	// The OpenGL context is now initialized...
	setDisplayCallback(displayCallback);
	setReshapeCallback(reshapeCallback);

	UTexture::enableTexturesClampToEdge();
	UTexture::enableLinearFilter();

	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);

	glClearColor(1.0f, 0.6f, 0.1f, 1.0f);


	// Check OpenGL version:
	std::cout << "OpenGL context" << std::endl;
	std::cout << "   version  . . : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "   vendor . . . : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "   renderer . . : " << glGetString(GL_RENDERER) << std::endl;
	
	//Init Light IDs:
	utopia::ULight::initIDs();
	
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

void utopia::Utopia::enableNearestFilter()
{
	updateAllTexturesParameteri(UTexture::enableNearestFilter);
}

void utopia::Utopia::enableNearestBipmapNearestFilter()
{
	updateAllTexturesParameteri(UTexture::enableNearestBipmapNearestFilter);
}

void utopia::Utopia::enableLinearFilter()
{
	updateAllTexturesParameteri(UTexture::enableLinearFilter);
}

void utopia::Utopia::enableLinearBipmapNearestFilter()
{
	updateAllTexturesParameteri(UTexture::enableLinearBipmapNearestFilter);
}

void utopia::Utopia::enableLinearBipmapLinearFilter()
{
	updateAllTexturesParameteri(UTexture::enableLinearBipmapLinearFilter);
}

void utopia::Utopia::enableTexturesRepeat()
{
	updateAllTexturesParameteri(UTexture::enableTexturesRepeat);
}

void utopia::Utopia::enableTexturesClampToEdge()
{
	updateAllTexturesParameteri(UTexture::enableTexturesClampToEdge);
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
	glClearColor(color.r,color.g,color.b,color.a);
}

std::shared_ptr<UMaterial> Utopia::getMaterialByName(const std::string& name)
{
	return m_pimpl->m_materials.at(name);
}

void Utopia::addMaterial(std::string name, std::shared_ptr<UMaterial> material)
{
	m_pimpl->m_materials.insert(std::pair<std::string, std::shared_ptr<UMaterial>>(name, material));
}

int Utopia::getWindowWidth()
{
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


std::shared_ptr<UTexture> Utopia::getTextureByName(const std::string& name)
{
	return m_pimpl->m_textures.at(name);
}
void Utopia::addTexture(const std::string& name, std::shared_ptr<UTexture> texture)
{
	m_pimpl->m_textures.insert(std::pair<std::string, std::shared_ptr<UTexture>>(name, texture));

	std::cout << "n textures: " << m_pimpl->m_textures.size() << std::endl;
}

bool Utopia::containTexture(const std::string& name)
{
	return m_pimpl->m_textures.count(name) >= 1;
}

bool Utopia::containMaterial(const std::string& name)
{
	return m_pimpl->m_materials.count(name) >= 1;
}

void Utopia::updateAllTexturesParameteri(void(*parametriSetMethod)(void))
{
	std::cout << "n textures to upload: " << m_pimpl->m_textures.size() << std::endl;

	for (const auto& kv : m_pimpl->m_textures)
	{
		kv.second->updateTextureParameteri(parametriSetMethod);
		std::cout<<"upload texture: " << kv.first << std::endl;
	}
}

void utopia::Utopia::updateAnisotropyLevelAllTextures(int value)
{
	std::cout << "n textures to upload the anisotropy level: " << m_pimpl->m_textures.size() << std::endl;

	for (const auto& kv : m_pimpl->m_textures)
	{
		kv.second->updateAnisotropyLevelTextureParameteri(value);
		std::cout << "upload texture: " << kv.first << std::endl;
	}
}

unsigned int Utopia::texturesMapSize()
{
	return (unsigned int)m_pimpl->m_textures.size();
}
unsigned int Utopia::materialsMapSize()
{
	return (unsigned int)m_pimpl->m_materials.size();
}

Utopia& Utopia::getInstance()
{
	static Utopia m_instance; // Guaranteed to be destroyed.
	// Instantiated on first use.
	return m_instance;
}




