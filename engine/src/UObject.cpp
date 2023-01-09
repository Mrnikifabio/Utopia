#include "UObject.h"

using namespace utopia;

unsigned int UObject::m_id_counter{ 0 };


struct UObject::pimpl {
	std::string m_name;
	unsigned int m_id;

	pimpl(const std::string& m_name, unsigned int m_id)
	{
		this->m_id = m_id;
		this->m_name = m_name;
	}
};

UObject::UObject(const std::string& name) : m_pimpl{ std::unique_ptr<pimpl>(new pimpl(name, UObject::m_id_counter++)) } {}

UObject::~UObject() noexcept = default;

void UObject::setName(const std::string& name)
{
	m_pimpl->m_name = name;
}

const std::string& UObject::getName() const
{
	return m_pimpl->m_name;
}

unsigned int UObject::getId() const
{
	return m_pimpl->m_id;
}
