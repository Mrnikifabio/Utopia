#include "UMaterial.h"
#include "UObject.h"
#include "utopia.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

struct UMaterial::pimpl
{

    glm::vec4 m_emission;
    glm::vec4 m_ambient;
    glm::vec4 m_diffuse;
    glm::vec4 m_specular;
    int m_shininess;

    pimpl()
    {
        this->m_emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        this->m_ambient = glm::vec4(0.2f,0.2f,0.2f,1.0f);
        this->m_diffuse = glm::vec4(0.0f,0.8f,0.8f,1.0f);
        this->m_specular = glm::vec4(0.5f,0.5f,0.5f,1.0f);
        m_shininess = 128;
    }

    pimpl(glm::vec4 emission, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, int shininess)
    {
        this->m_emission = emission;
        this->m_ambient = ambient;
        this->m_diffuse = diffuse;
        this->m_specular = specular;
        m_shininess = shininess;
    }

};

utopia::UMaterial::UMaterial(const std::string& name)
    :UObject{ name }, m_pimpl{ std::make_unique<pimpl>() } {}


utopia::UMaterial::UMaterial(const std::string& name, glm::vec4 emission, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, int shininess)
    :UObject{ name }, m_pimpl{ std::make_unique<pimpl>(emission, ambient, diffuse, specular, shininess) } {}

void utopia::UMaterial::render()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(m_pimpl->m_emission));
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(m_pimpl->m_ambient));
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(m_pimpl->m_diffuse));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(m_pimpl->m_specular));
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_pimpl->m_shininess);
}

void UMaterial::setEmission(glm::vec4 emission)
{
    m_pimpl.get()->m_emission = emission;
}
void UMaterial::setAmbient(glm::vec4 ambient)
{
    m_pimpl.get()->m_ambient = ambient;
}
void UMaterial::setDiffuse(glm::vec4 diffuse)
{
    m_pimpl.get()->m_diffuse = diffuse;
}
void UMaterial::setSpecular(glm::vec4 specular)
{
    m_pimpl.get()->m_specular = specular;
}
void UMaterial::setShininess(int shininess)
{
    m_pimpl.get()->m_shininess = shininess;
}

UMaterial::~UMaterial() = default;