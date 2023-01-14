#include "U2DObject.h"

using namespace utopia;

U2DObject::U2DObject(const std::string& name, const glm::vec2& position) : UObject(name), m_position{ position } {}
U2DObject::U2DObject(const std::string& name) : UObject(name) {}
U2DObject::~U2DObject() = default;

const glm::vec2& U2DObject::getPosition() const
{
	return m_position;
}

void U2DObject::setPosition(const glm::vec2& position)
{
	m_position = position;
}
