#include "UCamera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace utopia;

std::weak_ptr<UCamera> UCamera::m_mainCamera{ std::weak_ptr<UCamera>() };

struct UCamera::pimpl {
	glm::vec3 m_cameraPosition;
	pimpl() : m_cameraPosition{ glm::vec3(0, 0, 0) } {};
};

UCamera::UCamera(const std::string& name) 
	: m_pimpl{ std::make_unique<pimpl>() }, UNode{ name } {}

void UCamera::setCameraPosition(const glm::vec3& position)
{
	auto diff = position - m_pimpl->m_cameraPosition;
	m_pimpl->m_cameraPosition = position;
	setModelView(glm::translate(getModelView(), diff));
}

const glm::vec3& UCamera::getCameraPosition() const
{
	return m_pimpl->m_cameraPosition;
}


void UCamera::setMainCamera(std::weak_ptr<UCamera> camera)
{
	 m_mainCamera = camera;
}

std::weak_ptr<UCamera>& UCamera::getMainCamera()
{
	return m_mainCamera;
}

UCamera::~UCamera() = default;