/*****************************************************************//**
 * \file   Utopia.h
 *
 * \author Group10
 * \date   October 2022
 *********************************************************************/

#pragma once

 /////////////
 // VERSION //
 /////////////

	// Generic info:
#define LIB_NAME      "Utopia"  ///
#define LIB_VERSION   10                              ///< Library version (divide by 10)

 // Window size:
#define APP_WINDOWSIZEX   1024
#define APP_WINDOWSIZEY   512
#define APP_EYESIZEX      APP_WINDOWSIZEX / 2
#define APP_EYESIZEY      APP_WINDOWSIZEY / 1

#ifdef _WINDOWS 	
   // Export API:
   // Specifies i/o linkage (VC++ spec):
#ifdef UTOPIA_EXPORTS
#define LIB_API __declspec(dllexport)
#else
#define LIB_API __declspec(dllimport)
#endif      	
#else // Under Linux
#define LIB_API  // Dummy declaration
#endif

#include "U3DRenderPipeline.h"
#include "UProgramShader.h"
#include "UVertexShader.h"
#include "UFragmentShader.h"
#include "ovr.h"
#include <memory>

namespace utopia {

	class Utopia
	{

	private:
		struct pimpl;
		std::unique_ptr<pimpl> m_pimpl;
		Utopia();
		~Utopia();

	public:
		LIB_API static Utopia& getInstance();
		
		Utopia(Utopia const&) = delete;
		void operator=(Utopia const&) = delete;

		LIB_API bool init();
		LIB_API void clear();
		LIB_API void free();

		LIB_API void enableDepth();
		LIB_API void enableCullFace();
		LIB_API void enableLighting();
		LIB_API void enableShadeModel();
		LIB_API void enableWireFrameMode();
		LIB_API void enableSolidMode();

		LIB_API void setKeyboardCallback(void (*callback)(unsigned char, int, int));
		LIB_API void setDisplayCallback(void (*callback)(void));
		LIB_API void setReshapeCallback(void (*callback)(int, int));
		LIB_API void setSpecialCallback(void (*callback)(int, int, int));
		LIB_API void setCloseCallback(void(*callback)(void)); //invoked when the window is closed
		LIB_API void setPassiveMotionCallback(void (*callback)(int , int )); //invoked when the window is closed

		LIB_API void mainLoop();
		LIB_API bool isRunning();
		LIB_API void swap();

		LIB_API void setBackgroundColor(glm::vec4 color);

		LIB_API int getWindowWidth();
		LIB_API int getWindowHeight();

		LIB_API void setTimer(int timeoutMs, void(*callback)(int), int value);
		
		std::shared_ptr<UVertexShader> getBasicVertexShader();
		std::shared_ptr<UFragmentShader> getBasicFragmentShader();
		std::shared_ptr<UProgramShader> getBasicProgramShader(); //public but used only internally to the library
		std::shared_ptr<UVertexShader> getPassThroughVertShader();
		std::shared_ptr<UFragmentShader> getPassThroughFragmentShader();
		std::shared_ptr<UProgramShader> getPassThroughProgamShader();
		std::shared_ptr<OvVR> getOpenVRWrapper();

		void enableStereoscopic(const bool enable);
		LIB_API bool isStereoscopicEnabled();
	};

}
