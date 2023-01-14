#include "ULight.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>


using namespace utopia;

struct ULight::pimpl {
	
	glm::vec4 m_ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 m_diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 m_specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 m_gAmbient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	int m_lightID;
	
	pimpl(const int lightID)
	{
		this->m_lightID = lightID;
	}
};

std::stack<int> ULight::m_freeLightIDs = std::stack<int>();

void ULight::initIDs()
{
	GLint maxLights;
	glGetIntegerv(GL_MAX_LIGHTS, &maxLights);

	for (int i = 0; i < maxLights; i++)
	{
		m_freeLightIDs.push(GL_LIGHT0 + i);
	}
}

ULight::ULight(const std::string& name) : UNode{ name }
{
	if (m_freeLightIDs.empty())
	{
		throw std::runtime_error("No more lights available");
	}
	m_pimpl = std::unique_ptr<pimpl>(new pimpl(m_freeLightIDs.top()));
	m_freeLightIDs.pop();
}

ULight::~ULight() noexcept 
{
	m_freeLightIDs.push(m_pimpl->m_lightID);
	glDisable(m_pimpl->m_lightID);
}

void ULight::render()
{
	//render something somehow and then we make a call to UNode render, to render all of the children
		
	UNode::render();
	
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(m_pimpl->m_gAmbient));

	glLightfv(getLightID(), GL_AMBIENT, glm::value_ptr(m_pimpl->m_ambient));
	glLightfv(getLightID(), GL_DIFFUSE, glm::value_ptr(m_pimpl->m_diffuse));
	glLightfv(getLightID(), GL_SPECULAR, glm::value_ptr(m_pimpl->m_specular));

	glEnable(getLightID());
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