#include "UOrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace utopia;

UOrthographicCamera::UOrthographicCamera(const std::string& name, float right, float left, float top, float bottom, float far, float near) 
	: right{ right }, left{ left }, top{ top }, bottom{ bottom }, far{ far }, near{ near }, UCamera(name) {}

UOrthographicCamera::UOrthographicCamera(const std::string& name) : UOrthographicCamera(name, 10,-10, 10, -10, 1.0f, 100.0f) {}

const glm::mat4 UOrthographicCamera::getCameraMatrix() const
{
	return glm::ortho(left, right, bottom, top, near, far);
}