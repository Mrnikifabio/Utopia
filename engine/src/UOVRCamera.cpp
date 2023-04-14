#include "UOVRCamera.h"

using namespace utopia;

UOVRCamera::UOVRCamera(const std::string& name) : UCamera{ name }, m_cameraMatrix{ glm::mat4(1.0f) } {}

void UOVRCamera::setCameraMatrix(const glm::mat4& mat)
{
	m_cameraMatrix = mat;
}

const glm::mat4 UOVRCamera::getCameraMatrix() const
{
	return m_cameraMatrix;
}
