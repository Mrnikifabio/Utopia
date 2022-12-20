#include "UMesh.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

UMesh::UMesh(const std::string& name) : UNode{name} {}

void UMesh::pushLOD(std::unique_ptr<LOD>&& lod)
{
	m_lods.push_back(std::move(lod));
}

void UMesh::render()
{
	UNode::render();

	if (m_lods.size() > 0)
	{
		auto& lod = m_lods[0];

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


