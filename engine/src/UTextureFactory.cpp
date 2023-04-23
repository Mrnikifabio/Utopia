#include "UTextureFactory.h"
#include "Utopia.h"
#include "UTexture.h"

#include <GL/glew.h>
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

    auto texture = U2DTexture::load(name, UTexture::UTextureData(0, GL_RGBA, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap)));


	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();

	return texture;
}

std::shared_ptr<UCubeMapTexture> UTextureFactory::fromFileCubeMaps(const std::array<std::string, 6> names)
{
	FreeImage_Initialise();
    std::array<UTexture::UTextureData, 6> data;
    FIBITMAP* bitmaps[6];

    for (int curSide = 0; curSide < 6; curSide++)
    {
        // Load texture:
        FIBITMAP* fBitmap = FreeImage_Load(FreeImage_GetFileType(("assets/" + names[curSide]).c_str(), 0), ("assets/" + names[curSide]).c_str());
        bitmaps[curSide] = fBitmap;

        if (fBitmap == nullptr)
            std::cout << "[ERROR] loading file '" << names[curSide] << "'" << std::endl;
        int intFormat = GL_RGB;

        GLenum extFormat = GL_BGR;
        if (FreeImage_GetBPP(fBitmap) == 32)
        {
            intFormat = GL_RGBA;
            extFormat = GL_BGRA;
        }

        // Fix mirroring:
        FreeImage_FlipHorizontal(fBitmap);  // Correct mirroring from cube's inside
        FreeImage_FlipVertical(fBitmap);    // Correct JPG's upside-down

        data[curSide] = (UTexture::UTextureData(0, intFormat, FreeImage_GetWidth(fBitmap), FreeImage_GetHeight(fBitmap), 0, extFormat, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(fBitmap)));
    }

    auto texture = UCubeMapTexture::load(names[0], data);

    for(int curSide = 0; curSide < 6; curSide++)
        // Free resources:
        FreeImage_Unload(bitmaps[curSide]);

	FreeImage_DeInitialise();

	return texture;
}


