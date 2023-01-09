#include "UCamera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace utopia;

std::weak_ptr<UCamera> UCamera::m_mainCamera{ std::weak_ptr<UCamera>() };

struct UCamera::pimpl {
	glm::vec3 m_cameraPosition;
	float m_near;
	float m_far;
	pimpl(float far, float near) : m_cameraPosition{ glm::vec3(0, 0, 0) }, m_near{ near }, m_far{ far } {};
};

UCamera::UCamera(const std::string& name)
	: UNode{ name }, m_pimpl{ std::unique_ptr<pimpl>(new pimpl(100.0f, 1.0f)) } {}

void UCamera::setMainCamera(std::weak_ptr<UCamera> camera)
{
	 m_mainCamera = camera;
}

std::weak_ptr<UCamera>& UCamera::getMainCamera()
{
	return m_mainCamera;
}

void UCamera::setFar(const float far)
{
	m_pimpl->m_far = far;
}

void UCamera::setNear(const float near)
{
	m_pimpl->m_near = near;
}

float UCamera::getFar() const
{
	return m_pimpl->m_far;
}


float UCamera::getNear() const
{
	return m_pimpl->m_near;
}

UCamera::~UCamera() = default;
