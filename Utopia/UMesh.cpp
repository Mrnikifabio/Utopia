#include "UMesh.h"
#include "UMaterial.h"
#include "utopia.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

struct UMesh::pimpl {
	std::vector<std::unique_ptr<LOD>> m_lods;
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

UMesh::UMesh(const std::string& name) : m_pimpl{ std::make_unique<pimpl>() }, UNode{ name } {}
UMesh::UMesh(const std::string& name, std::weak_ptr<UMaterial> material) : m_pimpl { std::make_unique<pimpl>(material) }, UNode{ name } {}

void UMesh::pushLOD(std::unique_ptr<LOD>&& lod)
{
	m_pimpl->m_lods.push_back(std::move(lod));
}

std::weak_ptr<UMaterial> UMesh::getMaterial()
{
	if (m_pimpl.get()->m_material.expired())
	{
		m_pimpl.get()->m_material = Utopia::getInstance().getDefaultMaterial();
	}
	return m_pimpl.get()->m_material;
}


void UMesh::render()
{
	UNode::render();
	getMaterial().lock().get()->render();

	if (m_pimpl->m_lods.size() > 0)
	{
		auto& lod = m_pimpl->m_lods[0];

		for (auto& face : lod->faces)
		{
			glBegin(GL_TRIANGLES);
			for (auto& vertex : face.vertices)
			{
				glVertex3fv(glm::value_ptr(vertex->coord));
				glNormal3fv(glm::value_ptr(vertex->normal));
			}
			glEnd();
		}
	}
}

UMesh::~UMesh() = default;