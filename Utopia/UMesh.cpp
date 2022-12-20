#include "UMesh.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

struct UMesh::pimpl {
	std::vector<std::unique_ptr<LOD>> m_lods;
};

UMesh::UMesh(const std::string& name) : m_pimpl{std::make_unique<pimpl>()}, UNode{name} {}

void UMesh::pushLOD(std::unique_ptr<LOD>&& lod)
{
	m_pimpl->m_lods.push_back(std::move(lod));
}

void UMesh::render()
{
	UNode::render();

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