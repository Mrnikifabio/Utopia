#include "UObject.h"
#include "GL/glew.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include "UTexture.h"
#include "Utopia.h"

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
    glBindTexture(getGlTextureType(), m_pimpl.get()->m_texId);
    glTexParameteri(getGlTextureType(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(getGlTextureType(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void utopia::UTexture::enableNearestBitmapNearestFilter()
{
    glBindTexture(getGlTextureType(), m_pimpl.get()->m_texId);
    glTexParameteri(getGlTextureType(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(getGlTextureType(), GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
}

void utopia::UTexture::enableLinearFilter()
{
    glBindTexture(getGlTextureType(), m_pimpl.get()->m_texId);
    glTexParameteri(getGlTextureType(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(getGlTextureType(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void utopia::UTexture::enableLinearBitmapNearestFilter()
{
    glBindTexture(getGlTextureType(), m_pimpl.get()->m_texId);
    glTexParameteri(getGlTextureType(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(getGlTextureType(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

void utopia::UTexture::enableLinearBitmapLinearFilter()
{
    glBindTexture(getGlTextureType(), m_pimpl.get()->m_texId);
    glTexParameteri(getGlTextureType(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(getGlTextureType(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}


int utopia::UTexture::getMaxAnisotropicLevel()
{
    int anisotropicLevel;
    // Check for anistropic filtering extension:   
    if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
    {
        glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropicLevel);
        return anisotropicLevel;
    }
    else
    return 1;
}

void UTexture::render()
{
    glBindTexture(getGlTextureType(), m_pimpl.get()->m_texId);
}

void utopia::UTexture::updateAnisotropyLevel(int level)
{
    glBindTexture(getGlTextureType(), m_pimpl.get()->m_texId);

    int maxAnisotropy = UTexture::getMaxAnisotropicLevel();
    if (level > maxAnisotropy)
        level = maxAnisotropy;
    else
        if (level < 1)
            level = 1;

    glTexParameterf(getGlTextureType(), GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat)level);
}

unsigned int utopia::UTexture::getTexId()
{
    return m_pimpl->m_texId;
}

UTexture::~UTexture()
{
    unsigned int id = getTexId();
    glDeleteTextures(1, (GLuint*)(&id));
}