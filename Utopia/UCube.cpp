#include "UCube.h"
#include "UMaterial.h"
#include "utopia.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

struct UCube::pimpl {
	std::weak_ptr<UMaterial> m_material;

	pimpl()
	{
		m_material = Utopia::getInstance().getDefaultMaterial();
	}

	pimpl(std::weak_ptr<UMaterial> material)
	{
		m_material = material;
	}

};

UCube::UCube(const std::string& name) : m_pimpl{ std::make_unique<pimpl>() }, UNode{ name } {}
UCube::UCube(const std::string& name, std::weak_ptr<UMaterial> material) : m_pimpl{ std::make_unique<pimpl>(material) }, UNode{ name } {}


std::weak_ptr<UMaterial> UCube::getMaterial()
{
	if (m_pimpl.get()->m_material.expired())
	{
		m_pimpl.get()->m_material = Utopia::getInstance().getDefaultMaterial();
	}
	return m_pimpl.get()->m_material;
}

void UCube::setMaterial(std::weak_ptr<UMaterial> material)
{
	if (!material.expired())
		m_pimpl.get()->m_material = material;
}


void UCube::render()
{
	UNode::render();
	getMaterial().lock()->render();

	glutSolidCube(10);

	getMaterial().lock()->disable();


}

UCube::~UCube() = default;