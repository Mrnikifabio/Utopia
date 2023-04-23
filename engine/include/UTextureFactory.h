#pragma once
#include <iostream>
#include <memory>
#include <array>

#include "Utopia.h"
#include "U2DTexture.h"
#include "UCubeMapTexture.h"

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

namespace utopia
{
	class UTextureFactory
	{
	private:
		UTextureFactory();
		~UTextureFactory();

	public:
		LIB_API static UTextureFactory& getInstance();

		LIB_API UTextureFactory(UTextureFactory const&) = delete;
		void operator=(UTextureFactory const&) = delete;

		std::shared_ptr<U2DTexture> fromFile2D(const std::string& name);
		LIB_API std::shared_ptr<UCubeMapTexture> fromFileCubeMaps(const std::array<std::string, 6> names);
	};

}

