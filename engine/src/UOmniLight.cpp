#include "UOmniLight.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

struct UOmniLight::pimpl {
	float m_constantAttenuation;
	float m_linearAttenuation;
	float m_quadraticAttenuation;


	pimpl(const float constantAttenuation = 1, const float linearAttenuation = 0, const float quadraticAttenuation = 0)
	{
		this->m_constantAttenuation = constantAttenuation;
		this->m_linearAttenuation = linearAttenuation;
		this->m_quadraticAttenuation = quadraticAttenuation;
	}
};

UOmniLight::~UOmniLight() noexcept = default;

void UOmniLight::render()
{
	ULight::render();
	float cutoff = 180.0f;
	glLightfv(getLightID(), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	glLightfv(getLightID(), GL_SPOT_CUTOFF, &cutoff);
	glLightf(getLightID(), GL_CONSTANT_ATTENUATION, m_pimpl->m_constantAttenuation);
	glLightf(getLightID(), GL_LINEAR_ATTENUATION, m_pimpl->m_linearAttenuation);
	glLightf(getLightID(), GL_QUADRATIC_ATTENUATION, m_pimpl->m_quadraticAttenuation);
}

UOmniLight::UOmniLight(const std::string& name, const float constantAttenuation, const float linearAttenuation, const float quadraticAttenuation) : ULight{ name }, m_pimpl{ std::unique_ptr<pimpl>(new pimpl(constantAttenuation, linearAttenuation, quadraticAttenuation)) } {}

void UOmniLight::setConstantAttenuation(const float& constantAttenuation)
{
	m_pimpl->m_constantAttenuation = constantAttenuation;
}

auto UOmniLight::getConstantAttenuation() const -> const float&
{
	return m_pimpl->m_constantAttenuation;
}

void UOmniLight::setLinearAttenuation(const float& linearAttenuation)
{
	m_pimpl->m_linearAttenuation = linearAttenuation;
}

auto UOmniLight::getLinearAttenuation() const -> const float&
{
	return m_pimpl->m_linearAttenuation;
}

void UOmniLight::setQuadraticAttenuation(const float& quadraticAttenuation)
{
	m_pimpl->m_quadraticAttenuation = quadraticAttenuation;
}

auto UOmniLight::getQuadraticAttenuation() const -> const float&
{
	return m_pimpl->m_quadraticAttenuation;
}
