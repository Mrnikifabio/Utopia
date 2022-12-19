#include "ULight.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

struct ULight::pimpl {
	
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	int lightID;
	
	pimpl(const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, int lightID)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->lightID = lightID;
	}
};

ULight::ULight(const std::string& name, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, int lightID) : m_pimpl{ std::make_unique<pimpl>(ambient, diffuse, specular, lightID) }, UNode { name } {}

ULight::~ULight() noexcept = default;

void ULight::render()
{
	//render something somehow and then we make a call to UNode render, to render all of the children
	GLint maxLights;
	glGetIntegerv(GL_MAX_LIGHTS, &maxLights);
	
	UNode::render();
	glLightfv(getLightID(), GL_AMBIENT, glm::value_ptr(m_pimpl->ambient));
	glLightfv(getLightID(), GL_DIFFUSE, glm::value_ptr(m_pimpl->diffuse));
	glLightfv(getLightID(), GL_SPECULAR, glm::value_ptr(m_pimpl->specular));
}

void ULight::setAmbient(const glm::vec4& ambient)
{
	m_pimpl->ambient = ambient;
}

void ULight::setDiffuse(const glm::vec4& diffuse)
{
	m_pimpl->diffuse = diffuse;
}

void ULight::setSpecular(const glm::vec4& specular)
{
	m_pimpl->specular = specular;
}

auto ULight::getAmbient() const -> const glm::vec4&
{
	return m_pimpl->ambient;
}

auto ULight::getDiffuse() const -> const glm::vec4&
{
	return m_pimpl->diffuse;
}

auto ULight::getSpecular() const -> const glm::vec4&
{
	return m_pimpl->specular;
}

auto ULight::getLightID() const -> int
{
	return m_pimpl->lightID;
}