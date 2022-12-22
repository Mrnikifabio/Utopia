#include "UDirectionalLight.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

UDirectionalLight::~UDirectionalLight() noexcept = default;

UDirectionalLight::UDirectionalLight(const std::string& name, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, const glm::vec4& gAmbient) : ULight {name,ambient,diffuse,specular,gAmbient} {}

void UDirectionalLight::render()
{
	ULight::render();
	glLightfv(getLightID(), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)));
}