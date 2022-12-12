#include "UCamera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace utopia;

std::weak_ptr<UCamera> UCamera::m_mainCamera{ std::weak_ptr<UCamera>() };

UCamera::UCamera(const std::string& name) : UNode{ name }
{
}

UCamera::~UCamera() noexcept = default;

void UCamera::lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
	setModelView(glm::lookAt(eye,center,up));
}

void UCamera::setMainCamera(std::weak_ptr<UCamera> camera)
{
	 m_mainCamera = camera;
}

std::weak_ptr<UCamera>& UCamera::getMainCamera()
{
	return m_mainCamera;
}
