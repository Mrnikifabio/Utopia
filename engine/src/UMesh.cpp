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
		glDrawArrays(GL_TRIANGLES, 0, nOfPoints);
	}
	else {
		if (m_pimpl->m_lods.size() > 0)
		{
			auto& lod = m_pimpl->m_lods[0];
			unsigned int v = 0;
			unsigned int n = 0;
			unsigned int t = 0;
			float* vertices = new float[nOfPoints * 3];
			float* normals = new float[nOfPoints * 3];
			float* textureCoords = new float[nOfPoints * 2];
			for (auto& face : lod->faces)
			{
				for (auto& vertex : face.vertices)
				{
					vertices[v] = vertex->coord.x;
					v++;
					vertices[v] = vertex->coord.y;
					v++;
					vertices[v] = vertex->coord.z;
					v++;

					normals[n] = vertex->normal.x;
					n++;
					normals[n] = vertex->normal.y;
					n++;
					normals[n] = vertex->normal.z;
					n++;

					textureCoords[t] = vertex->uv.x;
					t++;
					textureCoords[t] = vertex->uv.y;
					t++;
				}
			}
			glGenBuffers(1, &v);
			glBindBuffer(GL_ARRAY_BUFFER, v);
			m_pimpl->m_vertexVbo = v;
			glBufferData(GL_ARRAY_BUFFER, nOfPoints * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
			delete[] vertices;

			glGenBuffers(1, &n);
			glBindBuffer(GL_ARRAY_BUFFER, n);
			m_pimpl->m_normalVbo = n;
			glBufferData(GL_ARRAY_BUFFER, nOfPoints * 3 * sizeof(float), normals, GL_STATIC_DRAW);
			delete[] normals;

			glGenBuffers(1, &t);
			glBindBuffer(GL_ARRAY_BUFFER, t);
			m_pimpl->m_textureCoordVbo = t;
			glBufferData(GL_ARRAY_BUFFER, nOfPoints * 2 * sizeof(float), textureCoords, GL_STATIC_DRAW);
			delete[] textureCoords;

			m_pimpl->m_vbo_loaded = true;
		}
	}

}

UMesh::~UMesh() = default;
