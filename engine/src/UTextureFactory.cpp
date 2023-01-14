#include "UTextureFactory.h"
#include "UTexture.h"
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

std::shared_ptr<UTexture> UTextureFactory::fromFile(const std::string& name)
{
	FreeImage_Initialise();

	FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(name.c_str(), 0), name.c_str());

	unsigned int texId;


	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	/*
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap),
		0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, // FreeImage uses BGR
		(void*)FreeImage_GetBits(bitmap));
	*/

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap), GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap));


	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();



	auto texture = std::shared_ptr<UTexture>(new UTexture(name, texId));
	Utopia::getInstance().addTexture(name, texture);
	return texture;
}
