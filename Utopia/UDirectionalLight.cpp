#include "UDirectionalLight.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

UDirectionalLight::~UDirectionalLight() noexcept = default;

UDirectionalLight::UDirectionalLight(const std::string& name) : ULight {name} {}

void UDirectionalLight::render()
{
	ULight::render();
	glLightfv(getLightID(), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)));
}