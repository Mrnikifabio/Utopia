#include "UMesh.h"
#include "UMaterial.h"
#include "Utopia.h"
#include <GL/glew.h>
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

struct UMesh::pimpl {
	std::vector<std::unique_ptr<LOD>> m_lods;
	std::shared_ptr<UMaterial> m_material;
	unsigned int m_vertexVbo;
	unsigned int m_normalVbo;
	unsigned int m_textureCoordVbo;
	unsigned int m_faceVbo;
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

void UMesh::pushLOD(std::unique_ptr<LOD>&& lod)
{
	m_pimpl->m_lods.push_back(std::move(lod));
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
	int nOfPoints = (int)m_pimpl->m_lods[0]->faces.size() * 3;
	if (m_pimpl->m_vbo_loaded) {
		glBindBuffer(GL_ARRAY_BUFFER, m_pimpl->m_vertexVbo);
		glVertexPointer(3, GL_FLOAT, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, m_pimpl->m_normalVbo);
		glNormalPointer(GL_FLOAT, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, m_pimpl->m_textureCoordVbo);
		glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pimpl->m_faceVbo);
		glDrawElements(GL_TRIANGLES, nOfPoints, GL_UNSIGNED_INT, nullptr);
	}
	else {
		//construct the first time all the vbo
		if (m_pimpl->m_lods.size() > 0)
		{
			auto& lod = m_pimpl->m_lods[0];
			unsigned int v = 0;
			unsigned int n = 0;
			unsigned int t = 0;
			unsigned int f = 0;
			float* vertices = new float[nOfPoints * 3];
			float* normals = new float[nOfPoints * 3];
			float* textureCoords = new float[nOfPoints * 2];
			unsigned int* faces = new unsigned int[nOfPoints];
			for (auto& face : lod->faces)
			{
				for (auto& vertex : face.vertices)
				{
					vertices[v++] = vertex->coord.x;
					vertices[v++] = vertex->coord.y;
					vertices[v++] = vertex->coord.z;

					normals[n++] = vertex->normal.x;
					normals[n++] = vertex->normal.y;
					normals[n++] = vertex->normal.z;

					textureCoords[t++] = vertex->uv.x;
					textureCoords[t++] = vertex->uv.y;

					faces[f] = f++;
				}
			}
			glGenBuffers(1, &m_pimpl->m_vertexVbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_pimpl->m_vertexVbo);
			glBufferData(GL_ARRAY_BUFFER, nOfPoints * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
			delete[] vertices;

			glGenBuffers(1, &m_pimpl->m_normalVbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_pimpl->m_normalVbo);
			glBufferData(GL_ARRAY_BUFFER, nOfPoints * 3 * sizeof(float), normals, GL_STATIC_DRAW);
			delete[] normals;

			glGenBuffers(1, &m_pimpl->m_textureCoordVbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_pimpl->m_textureCoordVbo);
			glBufferData(GL_ARRAY_BUFFER, nOfPoints * 2 * sizeof(float), textureCoords, GL_STATIC_DRAW);
			delete[] textureCoords;

			glGenBuffers(1, &m_pimpl->m_faceVbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pimpl->m_faceVbo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, nOfPoints * sizeof(unsigned int), faces, GL_STATIC_DRAW);

			m_pimpl->m_vbo_loaded = true;
		}
	}

}

UMesh::~UMesh() = default;
