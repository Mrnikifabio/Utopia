#include "UOmniLight.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;


UOmniLight::~UOmniLight() noexcept = default;

void UOmniLight::render()
{
	ULight::render();
	float cutoff = 180.0f;
	glLightfv(getLightID(), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	glLightfv(getLightID(), GL_SPOT_CUTOFF, &cutoff);
}

UOmniLight::UOmniLight(const std::string& name, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, int lightID) : ULight{ name, ambient, diffuse, specular, lightID } {}