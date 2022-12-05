/*****************************************************************//**
 * \file   utopia.cpp
 * \brief  Implementation of utopia.h
 *
 * \author Group 10
 * \date   October 2022
 *********************************************************************/

 //include
#include <string.h>
#include <iostream>
#include "utopia.h"
#include <gl/freeglut.h>


  // GLM:   
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


   // Projection matrices:
glm::mat4 perspective;



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



////////////////////////////////
// BODY OF CLASS Utopia //
////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This is the main rendering routine automatically invoked by FreeGLUT.
 */
void Utopia::display()
{

}

void displayCallback()
{

}


void reshapeCallback(int width, int height)
{

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1.0f, 100.0f);
	glLoadMatrixf(glm::value_ptr(projection));
	glMatrixMode(GL_MODELVIEW);

	Utopia::getInstance().clear();
	Utopia::getInstance().display();
	Utopia::getInstance().swap();

	std::cout << "[reshape func invoked] " << width<< " " << height << std::endl;

}


bool LIB_API Utopia::init()
{
	// Prevent double init:
	if (m_initFlag)
	{
		std::cout << "ERROR: class already initialized" << std::endl;
		return false;
	}

	// Init context:
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);

	char* myargv[1];
	int myargc = 1;
	myargv[0] = new char[6] {'U', 't', 'o', 'p', 'i', 'a'};
	glutInit(&myargc, myargv);

	// Set some optional flags:
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_EXIT);

	// Create the window with a specific title:
	glutCreateWindow("Utopia Window");

	// The OpenGL context is now initialized...
	setDisplayCallback(displayCallback);
	setReshapeCallback(reshapeCallback);

	glClearColor(1.0f, 0.6f, 0.1f, 1.0f);


	// Check OpenGL version:
	std::cout << "OpenGL context" << std::endl;
	std::cout << "   version  . . : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "   vendor . . . : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "   renderer . . : " << glGetString(GL_RENDERER) << std::endl;


	// Done:
	m_initFlag = true;
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
	if (!m_initFlag)
	{
		std::cout << "ERROR: class not initialized" << std::endl;
	}

	// Done:
	m_initFlag = false;
}

void Utopia::enableDepth()
{
	glEnable(GL_DEPTH_TEST);
}

void Utopia::enableCullFace()
{
	glEnable(GL_CULL_FACE);
}


void Utopia::enableLight0()
{
	glEnable(GL_LIGHT0);
}

void Utopia::enableLighting()
{
	glEnable(GL_LIGHTING);
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