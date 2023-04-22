#include "U2DTexture.h"

#include "Utopia.h"

#include "GL/glew.h"
#include <gl/freeglut.h>

using namespace utopia;

U2DTexture::U2DTexture(const std::string& name, unsigned int texId) : UTexture{ name, texId } {}

std::unordered_map<std::string, std::shared_ptr<U2DTexture>> U2DTexture::m_textures;
std::shared_ptr<U2DTexture> U2DTexture::m_defaultTexture;


std::shared_ptr<U2DTexture> U2DTexture::createWhiteTexture()
{
    GLubyte texData[] = { 255, 255, 255, 255 };
    UTextureData data = UTextureData(0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    return load("default", data);
}

std::shared_ptr<U2DTexture> U2DTexture::load(const std::string& name, const UTextureData& data)
{
    unsigned int texId;

    glGenTextures(1, &texId);
#ifdef _DEBUG
    std::cout << "texId: " << texId << std::endl;
#endif // _DEBUG
    glBindTexture(GL_TEXTURE_2D, texId);

    //gluBuild2DMipmaps(target, component, width, height, format, type, data);

    glTexImage2D(GL_TEXTURE_2D, data.level, data.internalformat, data.width, data.height, data.border, data.format, data.type, data.pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    auto texture = std::shared_ptr<U2DTexture>(new U2DTexture(name, texId));
    U2DTexture::add(name, texture);
    return texture;
}

unsigned int utopia::U2DTexture::getGlTextureType()
{
    return GL_TEXTURE_2D;
}

utopia::U2DTexture::~U2DTexture() = default;

const std::shared_ptr<U2DTexture> U2DTexture::getDefault()
{
    if (m_defaultTexture == nullptr)
        m_defaultTexture = createWhiteTexture();
    return m_defaultTexture;
}

void U2DTexture::enableTextureRepeat()
{
    glBindTexture(GL_TEXTURE_2D, getTexId());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
void U2DTexture::enableTextureClampToEdge()
{
    glBindTexture(GL_TEXTURE_2D, getTexId());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


void U2DTexture::forEach(std::function<void(std::shared_ptr<U2DTexture>)> apply)
{
#ifdef _DEBUG
    std::cout << "n textures to upload: " << m_textures.size() << std::endl;
#endif

    for (const auto& kv : m_textures)
    {
        apply(kv.second);
#ifdef _DEBUG
        std::cout << "upload texture: " << kv.first << std::endl;
#endif

    }
}

unsigned int U2DTexture::size()
{
    return (unsigned int)m_textures.size();
}

std::shared_ptr<U2DTexture> U2DTexture::getByName(const std::string& name)
{
    return m_textures.at(name);
}
void U2DTexture::add(const std::string& name, std::shared_ptr<U2DTexture> texture)
{
    m_textures.insert(std::pair<std::string, std::shared_ptr<U2DTexture>>(name, texture));

#ifdef _DEBUG
    std::cout << "n textures: " << m_textures.size() << std::endl;
#endif
}

bool U2DTexture::contains(const std::string& name)
{
    return m_textures.count(name) >= 1;
}