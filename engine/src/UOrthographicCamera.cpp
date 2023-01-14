#include "UOrthographicCamera.h"
#include "Utopia.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace utopia;

UOrthographicCamera::UOrthographicCamera(const std::string& name) : UCamera(name), m_right{ (float)Utopia::getInstance().getWindowWidth() }, m_left{0.0f}, m_top{ (float)Utopia::getInstance().getWindowHeight() }, m_bottom{0.0f}{}

const glm::mat4 UOrthographicCamera::getCameraMatrix() const
{
	return glm::ortho(m_left, m_right, m_bottom, m_top, getNear(), getFar());
}

float UOrthographicCamera::getRight() const
{
	return m_right;
}

float UOrthographicCamera::getLeft() const
{
	return m_left;
}

float UOrthographicCamera::getTop() const
{
	return m_top;
}

float UOrthographicCamera::getBottom() const
{
	return m_bottom;
}

void UOrthographicCamera::setRight(const float right)
{
	m_right = right;
}

void UOrthographicCamera::setLeft(const float left)
{
	m_left = left;
}

void UOrthographicCamera::setTop(const float top)
{
	m_top = top;
}

void UOrthographicCamera::setBottom(const float bottom)
{
	m_bottom = bottom;
}
