/*****************************************************************//**
 * \file   Utopia.h
 * \brief  Basic implementation of the gr. eng. lib.
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

#include "URenderPipeline.h"
#include "UMaterial.h"
#include <memory>
#include <unordered_map>

namespace utopia {

	class LIB_API Utopia
	{

	private:
		struct pimpl;
		std::unique_ptr<pimpl> m_pimpl;

		// Const/dest (as private to prevent instanciation):
		Utopia();
		~Utopia();

	public:

		static Utopia& getInstance();
		
		Utopia(Utopia const&) = delete;
		void operator=(Utopia const&) = delete;

		bool init();
		void clear();
		void free();

		void enableDepth();
		void enableCullFace();
		void enableLight0();
		void enableLighting();
		void enableWireFrameMode();
		void enableSolidMode();

		void setKeyboardCallback(void (*callback)(unsigned char, int, int));
		void setDisplayCallback(void (*callback)(void));
		void setReshapeCallback(void (*callback)(int, int));
		void setSpecialCallback(void (*callback)(int, int, int)); 
		void setCloseCallback(void(*callback)(void)); //invoked when the window is closed

		void display();
		void mainLoop();
		bool isRunning();
		void swap();

		URenderPipeline& getRenderPipeline();

		std::weak_ptr<UMaterial> getDefaultMaterial();
		std::weak_ptr<UMaterial> getMaterialByName(const std::string& name);
		void addMaterial(std::string name, std::shared_ptr<UMaterial> material);
		bool containMaterial(const std::string& name);


		std::shared_ptr<UTexture> getTextureByName(const std::string& name);
		void addTexture(const std::string&, std::shared_ptr<UTexture> texture);
		bool containTexture(const std::string& name);

		void updateAllTexturesParameteri(void(*parametriSetMethod)(void));
		
		int texturesMapSize();
		int materialsMapSize();

		int getWindowWidth();
		int getWindowHeight();

	
		
	};

}
