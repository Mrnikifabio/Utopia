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

void UMesh::setMaterial(std::weak_ptr<UMaterial> material)
{
	if(!material.expired())
		m_pimpl.get()->m_material = material;
}

void displayCube(float edge)
{
	float size = edge / 2.0f;
	float tiling = 1.f;

	// Texture mapping just on the cube:


	// Back:
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(size, -size, -size);
	glTexCoord2f(tiling, 0.0f);
	glVertex3f(-size, -size, -size);
	glTexCoord2f(0.0f, tiling);
	glVertex3f(size, size, -size);
	glTexCoord2f(tiling, tiling);
	glVertex3f(-size, size, -size);
	glEnd();

	// Front:
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size, size);
	glTexCoord2f(tiling, 0.0f);
	glVertex3f(size, -size, size);
	glTexCoord2f(0.0f, tiling);
	glVertex3f(-size, size, size);
	glTexCoord2f(tiling, tiling);
	glVertex3f(size, size, size);
	glEnd();

	// Left:
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size, -size);
	glTexCoord2f(tiling, 0.0f);
	glVertex3f(-size, -size, size);
	glTexCoord2f(0.0f, tiling);
	glVertex3f(-size, size, -size);
	glTexCoord2f(tiling, tiling);
	glVertex3f(-size, size, size);
	glEnd();

	// Right:
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(size, -size, -size);
	glTexCoord2f(tiling, 0.0f);
	glVertex3f(size, -size, size);
	glTexCoord2f(0.0f, tiling);
	glVertex3f(size, size, -size);
	glTexCoord2f(tiling, tiling);
	glVertex3f(size, size, size);
	glEnd();

	// Bottom:
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(size, -size, size);
	glTexCoord2f(tiling, 0.0f);
	glVertex3f(-size, -size, size);
	glTexCoord2f(0.0f, tiling);
	glVertex3f(size, -size, -size);
	glTexCoord2f(tiling, tiling);
	glVertex3f(-size, -size, -size);
	glEnd();

	// Top:
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(size, size, size);
	glTexCoord2f(tiling, 0.0f);
	glVertex3f(-size, size, size);
	glTexCoord2f(0.0f, tiling);
	glVertex3f(size, size, -size);
	glTexCoord2f(tiling, tiling);
	glVertex3f(-size, size, -size);
	glEnd();

	// Done with the cube, done with the texturing:
	//glDisable(GL_TEXTURE_2D);
}


void UMesh::render()
{
	UNode::render();
	getMaterial().lock()->render();

	if (m_pimpl->m_lods.size() > 0)
	{
		auto& lod = m_pimpl->m_lods[0];

		for (auto& face : lod->faces)
		{
			glBegin(GL_TRIANGLES);
			for (auto& vertex : face.vertices)
			{
				glNormal3fv(glm::value_ptr(vertex->normal));
				glTexCoord2fv(glm::value_ptr(vertex->uv));
				glVertex3fv(glm::value_ptr(vertex->coord));
			}
			glEnd();
		}
	}
	//displayCube(30);

	getMaterial().lock()->disable();


}

UMesh::~UMesh() = default;