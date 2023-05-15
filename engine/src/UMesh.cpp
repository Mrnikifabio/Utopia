#include "UMesh.h"
#include "UMaterial.h"
#include "Utopia.h"
#include <GL/glew.h>
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

struct UMesh::pimpl {
	std::vector<std::shared_ptr<LOD>> m_lods;
	std::shared_ptr<UMaterial> m_material;
	unsigned int m_vertexVbo;
	unsigned int m_normalVbo;
	unsigned int m_textureCoordVbo;
	unsigned int m_faceVbo;
	unsigned int m_vao;
	bool m_vbo_loaded = false;

	pimpl()
	{
		m_material = UMaterial::getDefaultMaterial();
	}

	pimpl(std::shared_ptr<UMaterial> material)
	{
		m_material = material;
	}

};

UMesh::UMesh(const std::string& name) : UNode{ name }, m_pimpl{ std::unique_ptr<pimpl>(new pimpl()) } {}
UMesh::UMesh(const std::string& name, std::shared_ptr<UMaterial> material) : UNode{ name }, m_pimpl { std::unique_ptr<pimpl>(new pimpl(material)) } {}

void UMesh::pushLOD(std::shared_ptr<LOD> lod)
{
	m_pimpl->m_lods.push_back(lod);
}

std::shared_ptr<UMaterial> UMesh::getMaterial()
{
	if (m_pimpl->m_material == nullptr)
	{
		m_pimpl->m_material = UMaterial::getDefaultMaterial();
	}
	return m_pimpl->m_material;
}

void UMesh::setMaterial(std::shared_ptr<UMaterial> material)
{
	m_pimpl.get()->m_material = material;
}


void UMesh::render()
{
	UNode::render();
	getMaterial()->render();
	if (m_pimpl->m_lods.size() > 0)
	{
		auto& lod = m_pimpl->m_lods[0];
		unsigned int nOfPoints = lod->nOfvertices;
		unsigned int nOfFaces = (unsigned int) lod->faces.size();

		if(nOfPoints > 0 && nOfFaces > 0) //somthing is off in the model?
		{
			if (!m_pimpl->m_vbo_loaded) {

				glGenVertexArrays(1, &m_pimpl->m_vao);
				glBindVertexArray(m_pimpl->m_vao);

				glGenBuffers(1, &m_pimpl->m_vertexVbo);
				glBindBuffer(GL_ARRAY_BUFFER, m_pimpl->m_vertexVbo);
				glBufferData(GL_ARRAY_BUFFER, nOfPoints * sizeof(UMesh::Vertex), &lod->vertices[0], GL_STATIC_DRAW);

				glBindVertexBuffer(0, m_pimpl->m_vertexVbo, 0, sizeof(UMesh::Vertex));

				glEnableVertexAttribArray(0);
				glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(UMesh::Vertex, coord)); // for vertex coord
				glVertexAttribBinding(0, 0);

				glEnableVertexAttribArray(1);
				glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(UMesh::Vertex, normal)); // for normal
				glVertexAttribBinding(1, 0);

				glEnableVertexAttribArray(2);
				glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(UMesh::Vertex, uv)); // for texture coord
				glVertexAttribBinding(2, 0);

				//glVertexAttribFormat(3, 4, GL_FLOAT, GL_FALSE, offsetof(UMesh::Vertex, tangent)); // for tangent coord
				//glVertexAttribBinding(3, 0);
				//glEnableVertexAttribArray(3);

				glGenBuffers(1, &m_pimpl->m_faceVbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pimpl->m_faceVbo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, nOfFaces * sizeof(UMesh::Face), &lod->faces[0], GL_STATIC_DRAW);

				glBindVertexArray(0);
				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(2);
				//glDisableVertexAttribArray(3);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				m_pimpl->m_vbo_loaded = true;
			}
			glBindVertexArray(m_pimpl->m_vao);
			glDrawElements(GL_TRIANGLES, nOfFaces * 3, GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);
		}
	}
}

UMesh::~UMesh()
{
	glDeleteBuffers(0, &m_pimpl->m_vertexVbo);
	glDeleteBuffers(1, &m_pimpl->m_faceVbo);
	glDeleteVertexArrays(1, &m_pimpl->m_vao);
}
