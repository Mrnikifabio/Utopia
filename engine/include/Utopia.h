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
#include "U2DRenderPipeline.h"
#include "UMaterial.h"
#include "UTexture.h"
#include <memory>
#include <unordered_map>

namespace utopia {

	class Utopia
	{

	private:
		struct pimpl;
		std::unique_ptr<pimpl> m_pimpl;
		void updateAllTexturesParameteri(void(*parametriSetMethod)(void));
		// Const/dest (as private to prevent instanciation):
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

		LIB_API void enableNearestFilter();
		LIB_API void enableNearestBipmapNearestFilter();
		LIB_API void enableLinearFilter();
		LIB_API void enableLinearBipmapNearestFilter();
		LIB_API void enableLinearBipmapLinearFilter();
		LIB_API void enableTexturesRepeat();
		LIB_API void enableTexturesClampToEdge();

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

		LIB_API std::shared_ptr<UMaterial> getMaterialByName(const std::string& name);
		LIB_API void addMaterial(std::string name, std::shared_ptr<UMaterial> material);
		LIB_API bool containMaterial(const std::string& name);


		LIB_API std::shared_ptr<UTexture> getTextureByName(const std::string& name);
		LIB_API void addTexture(const std::string&, std::shared_ptr<UTexture> texture);
		LIB_API bool containTexture(const std::string& name);

		LIB_API void updateAnisotropyLevelAllTextures(int value);

		
		LIB_API unsigned int texturesMapSize();
		LIB_API unsigned int materialsMapSize();

		LIB_API int getWindowWidth();
		LIB_API int getWindowHeight();

		LIB_API void setTimer(int timeoutMs, void(*callback)(int), int value);

	};

}
