#include "UMaterial.h"
#include "UObject.h"
#include "U2DTexture.h"
#include "Utopia.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

std::shared_ptr<UMaterial> UMaterial::m_defaultMaterial;
std::unordered_map<std::string, std::shared_ptr<UMaterial>> UMaterial::m_materials;

struct UMaterial::pimpl
{

    glm::vec4 m_emission;
    glm::vec4 m_ambient;
    glm::vec4 m_diffuse;
    glm::vec4 m_specular;
    int m_shininess;

    std::shared_ptr<U2DTexture> m_texture;

    pimpl()
    {
        this->m_emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        this->m_ambient = glm::vec4(0.1f,0.1f,0.1f,1.0f);
        this->m_diffuse = glm::vec4(0.7f,0.7f,0.7f,1.0f);
        this->m_specular = glm::vec4(0.6f,0.6f,0.6f,1.0f);
        m_shininess = 128;
        m_texture = U2DTexture::getDefault();
    }

    pimpl(glm::vec4 emission, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, int shininess)
    {
        this->m_emission = emission;
        this->m_ambient = ambient;
        this->m_diffuse = diffuse;
        this->m_specular = specular;
        m_shininess = shininess;
        m_texture = U2DTexture::getDefault();
    }

};

UMaterial::UMaterial(const std::string& name)
    :UObject{ name }, m_pimpl{ std::unique_ptr<pimpl>(new pimpl()) } {}


UMaterial::UMaterial(const std::string& name, const glm::vec4& emission, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, int shininess)
    :UObject{ name }, m_pimpl{ std::unique_ptr<pimpl>(new pimpl(emission, ambient, diffuse, specular, shininess)) } {}



void utopia::UMaterial::render()
{
    auto matEmission = UProgramShader::getActiveProgramShader()->getParamLocation("matEmission");
    UProgramShader::getActiveProgramShader()->setVec3(matEmission, glm::vec3(m_pimpl->m_emission));

    auto matAmbient = UProgramShader::getActiveProgramShader()->getParamLocation("matAmbient");
    UProgramShader::getActiveProgramShader()->setVec3(matAmbient, glm::vec3(m_pimpl->m_ambient));

    auto matDiffuse = UProgramShader::getActiveProgramShader()->getParamLocation("matDiffuse");
    UProgramShader::getActiveProgramShader()->setVec3(matDiffuse, glm::vec3(m_pimpl->m_diffuse));

    auto matSpecular = UProgramShader::getActiveProgramShader()->getParamLocation("matSpecular");
    UProgramShader::getActiveProgramShader()->setVec3(matSpecular, glm::vec3(m_pimpl->m_specular));

    auto matShininess = UProgramShader::getActiveProgramShader()->getParamLocation("matShininess");
    UProgramShader::getActiveProgramShader()->setFloat(matShininess, (GLfloat)m_pimpl->m_shininess);

    m_pimpl->m_texture->render();
}

void UMaterial::setEmission(const glm::vec4& emission)
{
    m_pimpl->m_emission = emission;
}
void UMaterial::setAmbient(const glm::vec4& ambient)
{
    m_pimpl->m_ambient = ambient;
}
void UMaterial::setDiffuse(const glm::vec4& diffuse)
{
    m_pimpl->m_diffuse = diffuse;
}
void UMaterial::setSpecular(const glm::vec4& specular)
{
    m_pimpl->m_specular = specular;
}
void UMaterial::setShininess(int shininess)
{
    m_pimpl->m_shininess = shininess;
}

void UMaterial::setTexture(std::shared_ptr<U2DTexture> texture)
{
    m_pimpl->m_texture = texture;
}

const std::shared_ptr<UMaterial> UMaterial::getDefaultMaterial()
{
    if(m_defaultMaterial==nullptr)
        m_defaultMaterial = std::make_shared<UMaterial>("default");
    return m_defaultMaterial;
}


bool UMaterial::contains(const std::string& name)
{
    return m_materials.count(name) >= 1;
}

unsigned int UMaterial::size()
{
    return (unsigned int)m_materials.size();
}

void utopia::UMaterial::forEach(std::function<void(std::shared_ptr<UMaterial>)> apply)
{
#ifdef _DEBUG
    std::cout << "n materials to upload: " << m_materials.size() << std::endl;
#endif

    for (const auto& kv : m_materials)
    {
        apply(kv.second);
#ifdef _DEBUG
        std::cout << "upload material: " << kv.first << std::endl;
#endif

    }
}

std::shared_ptr<UMaterial> UMaterial::getByName(const std::string& name)
{
    return m_materials.at(name);
}

void UMaterial::add(std::string name, std::shared_ptr<UMaterial> material)
{
    m_materials.insert(std::pair<std::string, std::shared_ptr<UMaterial>>(name, material));
}

UMaterial::~UMaterial() = default;