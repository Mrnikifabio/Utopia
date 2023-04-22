#include "UCubeMapTexture.h"

#include "GL/glew.h"
#include <gl/freeglut.h>

using namespace utopia;

UCubeMapTexture::UCubeMapTexture(const std::string& name, unsigned int texId) : UTexture{ name, texId } {}


std::shared_ptr<UCubeMapTexture> UCubeMapTexture::load(const std::string& name, const std::array<UTextureData, 6>& data)
{
    unsigned int texId;

    glGenTextures(1, &texId);
#ifdef _DEBUG
    std::cout << "texId: " << texId << std::endl;
#endif // _DEBUG

    glBindTexture(GL_TEXTURE_2D, texId);

    for (int curSide = 0; curSide < 6; curSide++)
    {
        const auto& d = data[curSide];
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + curSide, d.level, d.internalformat, d.width, d.height, d.border, d.format, d.type, d.pixels);
    }

    return std::shared_ptr<UCubeMapTexture>(new UCubeMapTexture(name, texId));
}

unsigned int utopia::UCubeMapTexture::getGlTextureType()
{
    return GL_TEXTURE_CUBE_MAP;
}

UCubeMapTexture::~UCubeMapTexture() = default;

void UCubeMapTexture::enableTextureRepeat()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, getTexId());
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
}
void UCubeMapTexture::enableTextureClampToEdge()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, getTexId());
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}