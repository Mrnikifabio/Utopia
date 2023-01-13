#include "UTexture.h"
#include "UObject.h"
#include "Utopia.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

/////////////
// #DEFINE //
/////////////

   // Quick define for the extension:
#define GL_TEXTURE_MAX_ANISOTROPY_EXT        0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT    0x84FF
// See https://www.opengl.org/registry/specs/EXT/texture_filter_anisotropic.txt

// Silly Microsoft GL.h stuck at version 1.1...:
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

using namespace utopia;

struct UTexture::pimpl
{

    unsigned int m_texId;

    pimpl(unsigned int texId)
    {
        m_texId = texId;
    }

};



UTexture::UTexture(const std::string& name, unsigned int texId)
    :UObject{ name }, m_pimpl{ std::unique_ptr<pimpl>(new pimpl(texId)) } {}

void utopia::UTexture::enableNearestFilter()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void utopia::UTexture::enableNearestBipmapNearestFilter()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
}

void utopia::UTexture::enableLinearFilter()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void utopia::UTexture::enableLinearBipmapNearestFilter()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

void utopia::UTexture::enableLinearBipmapLinearFilter()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}


int utopia::UTexture::getMaxAnisotropicLevel()
{
    int anisotropicLevel;
    // Check for anistropic filtering extension:   
    if (strstr((const char*)glGetString(GL_EXTENSIONS), "GL_EXT_texture_filter_anisotropic"))
    {
        glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropicLevel);
        return anisotropicLevel;
    }
    else
    return 1;
}

void UTexture::render()
{
    glBindTexture(GL_TEXTURE_2D, m_pimpl.get()->m_texId);
    glEnable(GL_TEXTURE_2D);
}

void UTexture::enableTexturesRepeat()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
void UTexture::enableTexturesClampToEdge()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void UTexture::setAnisotropyLevel(int level)
{
    int maxAnisotropy = UTexture::getMaxAnisotropicLevel();
    if (level > maxAnisotropy)
        level = maxAnisotropy;
    else
        if (level < 1)
            level = 1;

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat)level);
}

void utopia::UTexture::updateTextureParameteri(void(*parametriSetMethod)(void))
{
    glBindTexture(GL_TEXTURE_2D, m_pimpl.get()->m_texId);
    glEnable(GL_TEXTURE_2D);
    parametriSetMethod();
    glDisable(GL_TEXTURE_2D);
}

void utopia::UTexture::updateAnisotropyLevelTextureParameteri(int value)
{
    glBindTexture(GL_TEXTURE_2D, m_pimpl.get()->m_texId);
    glEnable(GL_TEXTURE_2D);
    setAnisotropyLevel(value);
    glDisable(GL_TEXTURE_2D);
}




UTexture::~UTexture() = default;