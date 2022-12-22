#include "UTexture.h"
#include "UObject.h"
#include "utopia.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


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
    :UObject{ name }, m_pimpl{ std::make_unique<pimpl>(texId) } {}

void UTexture::render()
{
    glBindTexture(GL_TEXTURE_2D, m_pimpl.get()->m_texId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glEnable(GL_TEXTURE_2D);
}





UTexture::~UTexture() = default;