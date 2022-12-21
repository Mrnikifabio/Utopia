#include "ULight.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

struct ULight::pimpl {
	
	glm::vec4 m_ambient;
	glm::vec4 m_diffuse;
	glm::vec4 m_specular;
	glm::vec4 m_gAmbient;
	int m_lightID;
	
	pimpl(const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular,const glm::vec4& gAmbient,const int lightID)
	{
		this->m_ambient = ambient;
		this->m_diffuse = diffuse;
		this->m_specular = specular;
		this->m_gAmbient = gAmbient;
		this->m_lightID = lightID;
	}
};

ULight::ULight(const std::string& name, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, const glm::vec4& gAmbient, const int lightID) : m_pimpl{ std::make_unique<pimpl>(ambient, diffuse, specular, gAmbient, lightID) }, UNode { name } {}

ULight::~ULight() noexcept = default;

void ULight::render()
{
	//render something somehow and then we make a call to UNode render, to render all of the children
	GLint maxLights;
	glGetIntegerv(GL_MAX_LIGHTS, &maxLights);
	
	UNode::render();
	/*
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(m_pimpl->m_gAmbient));
	*/
	glLightfv(getLightID(), GL_AMBIENT, glm::value_ptr(m_pimpl->m_ambient));
	glLightfv(getLightID(), GL_DIFFUSE, glm::value_ptr(m_pimpl->m_diffuse));
	glLightfv(getLightID(), GL_SPECULAR, glm::value_ptr(m_pimpl->m_specular));
}

void ULight::setAmbient(const glm::vec4& ambient)
{
	m_pimpl->m_ambient = ambient;
}

void ULight::setDiffuse(const glm::vec4& diffuse)
{
	m_pimpl->m_diffuse = diffuse;
}

void ULight::setSpecular(const glm::vec4& specular)
{
	m_pimpl->m_specular = specular;
}

auto ULight::getAmbient() const -> const glm::vec4&
{
	return m_pimpl->m_ambient;
}

auto ULight::getDiffuse() const -> const glm::vec4&
{
	return m_pimpl->m_diffuse;
}

auto ULight::getSpecular() const -> const glm::vec4&
{
	return m_pimpl->m_specular;
}

auto ULight::getLightID() const -> int
{
	return m_pimpl->m_lightID;
}

void ULight::setGlobalAmbient(const glm::vec4& gAmbient)
{
	m_pimpl->m_gAmbient = gAmbient;
}

auto ULight::getGlobalAmbient() const -> const glm::vec4&
{
	return m_pimpl->m_gAmbient;
}