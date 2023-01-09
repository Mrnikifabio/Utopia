#include "USpotLight.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

struct USpotLight::pimpl {
	float m_cutoff;
	glm::vec3 m_direction;
	float m_constantAttenuation;
	float m_linearAttenuation;
	float m_quadraticAttenuation;


	pimpl(const float cutoff, const glm::vec3& direction, const float constantAttenuation, const float linearAttenuation, const float quadraticAttenuation)
	{
		this->m_cutoff = cutoff;
		this->m_direction = direction;
		this->m_constantAttenuation = constantAttenuation;
		this->m_linearAttenuation = linearAttenuation;
		this->m_quadraticAttenuation = quadraticAttenuation;
	}
};

USpotLight::~USpotLight() noexcept = default;

void USpotLight::render()
{
	ULight::render();
	glLightfv(getLightID(), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	glLightfv(getLightID(), GL_SPOT_CUTOFF, &m_pimpl->m_cutoff);
	glLightfv(getLightID(), GL_SPOT_DIRECTION, glm::value_ptr(m_pimpl->m_direction));
	glLightf(getLightID(), GL_CONSTANT_ATTENUATION, m_pimpl->m_constantAttenuation);
	glLightf(getLightID(), GL_LINEAR_ATTENUATION, m_pimpl->m_linearAttenuation);
	glLightf(getLightID(), GL_QUADRATIC_ATTENUATION, m_pimpl->m_quadraticAttenuation);
}

USpotLight::USpotLight(const std::string& name, const float cutoff, const glm::vec3& direction, const float constantAttenuation, const float linearAttenuation, const float quadraticAttenuation) : ULight{ name }, m_pimpl{ std::unique_ptr<pimpl>(new pimpl(cutoff, direction, constantAttenuation, linearAttenuation, quadraticAttenuation)) } {}

void USpotLight::setDirection(const glm::vec3& direction)
{
	m_pimpl->m_direction = direction;
}

auto USpotLight::getDirection() const -> const glm::vec3&
{
	return m_pimpl->m_direction;
}

void USpotLight::setCutoff(const float& cutoff)
{
	m_pimpl->m_cutoff = cutoff;
}

auto USpotLight::getCutoff() const -> const float&
{
	return m_pimpl->m_cutoff;
}

void USpotLight::setConstantAttenuation(const float& constantAttenuation)
{
	m_pimpl->m_constantAttenuation = constantAttenuation;
}

auto USpotLight::getConstantAttenuation() const -> const float&
{
	return m_pimpl->m_constantAttenuation;
}

void USpotLight::setLinearAttenuation(const float& linearAttenuation)
{
	m_pimpl->m_linearAttenuation = linearAttenuation;
}

auto USpotLight::getLinearAttenuation() const -> const float&
{
	return m_pimpl->m_linearAttenuation;
}

void USpotLight::setQuadraticAttenuation(const float& quadraticAttenuation)
{
	m_pimpl->m_quadraticAttenuation = quadraticAttenuation;
}

auto USpotLight::getQuadraticAttenuation() const -> const float&
{
	return m_pimpl->m_quadraticAttenuation;
}
