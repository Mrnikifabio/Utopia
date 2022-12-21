#include "USpotLight.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

struct USpotLight::pimpl {
	float cutoff;
	glm::vec3 direction;
	
	pimpl(const float cutoff, const glm::vec3& direction)
	{
		this->cutoff = cutoff;
		this->direction = direction;
	}
};

USpotLight::~USpotLight() noexcept = default;

void USpotLight::render()
{
	ULight::render();
	glLightfv(getLightID(), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	glLightfv(getLightID(), GL_SPOT_CUTOFF, &m_pimpl->cutoff);
	glLightfv(getLightID(), GL_SPOT_DIRECTION, glm::value_ptr(m_pimpl->direction));
}

USpotLight::USpotLight(const std::string& name, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, int lightID, float cutoff, const glm::vec3& direction) : m_pimpl{ std::make_unique<pimpl>(cutoff, direction) }, ULight{ name, ambient, diffuse, specular, lightID } {}	

void USpotLight::setDirection(const glm::vec3& direction)
{
	m_pimpl->direction = direction;
}

auto USpotLight::getDirection() const -> const glm::vec3&
{
	return m_pimpl->direction;
}

void USpotLight::setCutoff(const float& cutoff)
{
	m_pimpl->cutoff = cutoff;
}

auto USpotLight::getCutoff() const -> const float&
{
	return m_pimpl->cutoff;
}