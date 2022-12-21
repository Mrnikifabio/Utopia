#include "UPerspectiveCamera.h"
#include "utopia.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace utopia;

UPerspectiveCamera::UPerspectiveCamera(const std::string& name) : m_fovRadians{ glm::radians(45.0f) }, UCamera(name) {}

const glm::mat4 UPerspectiveCamera::getCameraMatrix() const
{
	auto aspect = (float)Utopia::getInstance().getWindowWidth() / (float)Utopia::getInstance().getWindowHeight();
	return glm::perspective(m_fovRadians, aspect, getNear(), getFar());
}

float UPerspectiveCamera::getFovRadians() const
{
	return m_fovRadians;
}

void UPerspectiveCamera::setFovRadians(float fovRadians)
{
	m_fovRadians = fovRadians;
}

