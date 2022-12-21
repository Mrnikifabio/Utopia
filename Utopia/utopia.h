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

	public:

		static Utopia& getInstance()
		{
			static Utopia m_instance; // Guaranteed to be destroyed.
			// Instantiated on first use.
			return m_instance;
		}

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
		std::unordered_map<std::string, std::shared_ptr<UMaterial>> getMaterialsMap();
		std::weak_ptr<UMaterial> getDefaultMaterial();
		std::weak_ptr<UMaterial> getMaterialByName(std::string name);
		void addMaterial(std::string name, std::shared_ptr<UMaterial> material);


		int getWindowWidth();
		int getWindowHeight();

	private:
		// Const/dest (as private to prevent instanciation):
		Utopia() : m_initFlag{ false }, m_renderPipeline{ std::make_unique<URenderPipeline>("renderPipeline") }, m_defaultMaterial{std::make_shared<UMaterial>("default")} {};
		~Utopia() {}
		std::unique_ptr<URenderPipeline> m_renderPipeline;
		std::unordered_map<std::string, std::shared_ptr<UMaterial>> m_materials;
		std::shared_ptr<UMaterial> m_defaultMaterial;
		
		bool m_initFlag;
	};

}
