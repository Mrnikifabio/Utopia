#include "UPerspectiveCamera.h"
#include "utopia.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace utopia;

UPerspectiveCamera::UPerspectiveCamera(const std::string& name, float fovRadians, float near, float far) 
	: fovRadians{ fovRadians }, near{ near }, far{ far }, UCamera(name) {}

UPerspectiveCamera::UPerspectiveCamera(const std::string& name) : UPerspectiveCamera(name, glm::radians(45.0f), 1.0f, 100.0f){}

const glm::mat4& UPerspectiveCamera::getCameraMatrix() const
{
	auto aspect = (float)Utopia::getInstance().getWindowWidth() / (float)Utopia::getInstance().getWindowHeight();
	return glm::perspective(fovRadians, aspect, near, far);
}

