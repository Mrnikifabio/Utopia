#include "UTextureFactory.h"
#include "Utopia.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

#include <FreeImage.h>


using namespace utopia;

UTextureFactory::UTextureFactory() {};
UTextureFactory::~UTextureFactory() {};

UTextureFactory& UTextureFactory::getInstance()
{
	static UTextureFactory m_instance;
	return m_instance;
}

std::shared_ptr<U2DTexture> UTextureFactory::fromFile2D(const std::string& name)
{
	FreeImage_Initialise();

	FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(name.c_str(), 0), name.c_str());

	auto texture = U2DTexture::loadTexture(name, GL_RGBA, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap), GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap));


	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();

	return texture;
}


