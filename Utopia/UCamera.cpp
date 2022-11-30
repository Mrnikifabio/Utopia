#include "UCamera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace utopia;

std::weak_ptr<UCamera> UCamera::m_mainCamera{ std::weak_ptr<UCamera>() };

struct UCamera::pimpl {
	glm::mat4 m_projectMatrix;
	pimpl(const glm::mat4& projectMatrix)
	{
		m_projectMatrix = projectMatrix;
	}
};

UCamera::UCamera(const std::string& name) : m_pimpl{ std::make_unique<pimpl>(glm::mat4(1)) }, UNode{ name }
{
}

UCamera::~UCamera() noexcept = default;

void UCamera::lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
	m_pimpl->m_projectMatrix = glm::lookAt(eye,center,up);
}

const glm::mat4& UCamera::getProjectionMatrix() const
{
	return m_pimpl->m_projectMatrix;
}

void UCamera::setMainCamera(std::weak_ptr<UCamera> camera)
{
	 m_mainCamera = camera;
}

std::weak_ptr<UCamera>& UCamera::getMainCamera()
{
	return m_mainCamera;
}
