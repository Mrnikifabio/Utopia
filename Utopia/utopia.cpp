/*****************************************************************//**
 * \file   utopia.cpp
 * \brief  Implementation of utopia.h
 *
 * \author Group 10
 * \date   October 2022
 *********************************************************************/

//include
#include <string.h>
#include "utopia.h"
#include <gl/freeglut.h>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>




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
void displayCallback()
{
	// Clear the screen:
	glClearColor(1.0f, 0.6f, 0.1f, 1.0f); // RGBA components
	glClear(GL_COLOR_BUFFER_BIT);

	// Swap this context's buffer:
	glutSwapBuffers();

	// Force another invocation of this rendering callback:
	glutPostWindowRedisplay(glutGetWindow());
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This callback is invoked each time the window gets resized (and once also when created).
 * @param width new window width
 * @param height new window height
 */
void reshapeCallback(int width, int height)
{
	// For your information...:
	std::cout << "[reshape func invoked]" << std::endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Initialization method. Call this before any other Eureka function.
 * @return true on success, false on error
 */
bool LIB_API Utopia::init()
{
	// Prevent double init:
	if (m_initFlag)
	{
		std::cout << "ERROR: class already initialized" << std::endl;
		return false;
	}

	// Init context:
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);

	char* myargv[1];
	int myargc = 1;
	myargv[0] = new char[6]{'U','t','o','p','i','a'};
	glutInit(&myargc, myargv);

	// Set some optional flags:
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Create the window with a specific title:
	glutCreateWindow("Utopia Window");

	// The OpenGL context is now initialized...



	// Check OpenGL version:
	std::cout << "OpenGL context" << std::endl;
	std::cout << "   version  . . : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "   vendor . . . : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "   renderer . . : " << glGetString(GL_RENDERER) << std::endl;

	// Done:
	m_initFlag = true;
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Deinitialization method.
 * @return true on success, false on error
 */
bool LIB_API Utopia::free()
{
	// Really necessary?
	if (!m_initFlag)
	{
		std::cout << "ERROR: class not initialized" << std::endl;
		return false;
	}

	// Done:
	m_initFlag = false;
	return true;
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

void Utopia::setTestCallBack()
{
	setDisplayCallback(displayCallback);
	setReshapeCallback(reshapeCallback);
}

void Utopia::mainLoop()
{
	// Enter the main FreeGLUT processing loop:
	glutMainLoop();

	// Done:
	std::cout << "[application terminated]" << std::endl;
}

bool Utopia::isRunning()
{
	return true;
}

void Utopia::swap()
{
	glutSwapBuffers();
}