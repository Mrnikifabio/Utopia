#include <UMesh.h>
#include <U2DQuad.h>
#include <GL/glew.h>
#include <UFbo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Utopia.h>

using namespace utopia;

struct U2DQuad::pimpl {
	glm::vec2 m_bottomLeftPosition;
	glm::vec2 m_topRightPosition;
	unsigned int m_vao = -1;
	unsigned int m_texId = -1;
	unsigned int m_orthoWidth;
	unsigned int m_orthoHeight;
	glm::mat4 m_positionMatrix;

	std::unique_ptr<UFbo> m_fbo;
	pimpl(const std::string& name,
		const glm::vec2& bottomLeftPosition, 
		const glm::vec2& topRightPosition, 
		const unsigned int orthoWidth, 
		const unsigned int orthoHeight,
		const glm::mat4& postionMatrix) : 
		m_bottomLeftPosition{ bottomLeftPosition }, 
		m_topRightPosition{ topRightPosition },
		m_orthoWidth{orthoWidth},
		m_orthoHeight{orthoHeight},
		m_positionMatrix{postionMatrix}
{}
};

U2DQuad::U2DQuad(const std::string& name, 
	const glm::vec2& startPosition, 
	const glm::vec2& endPosition,
	const unsigned int orthoWidth, 
	const unsigned int orthoHeight,
	const glm::mat4& postionMatrix) :
	UObject(name),
	m_pimpl{ std::unique_ptr<pimpl>(new pimpl(name, startPosition, endPosition, orthoWidth, orthoHeight, postionMatrix)) } {}

U2DQuad::~U2DQuad() = default;

bool U2DQuad::init()
{
	glGenVertexArrays(1, &m_pimpl->m_vao);
	glBindVertexArray(m_pimpl->m_vao);

	glm::vec2 v1 = m_pimpl->m_bottomLeftPosition;
	glm::vec2 v2 = glm::vec2(m_pimpl->m_topRightPosition.x, m_pimpl->m_bottomLeftPosition.y);
	glm::vec2 v3 = glm::vec2(m_pimpl->m_bottomLeftPosition.x, m_pimpl->m_topRightPosition.y);
	glm::vec2 v4 = m_pimpl->m_topRightPosition;
	std::vector<glm::vec2> quadCoord = { v1, v2, v3, v4 };

	unsigned int quadCoordVbo;
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &quadCoordVbo);
	glBindBuffer(GL_ARRAY_BUFFER, quadCoordVbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), &quadCoord[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	std::vector<glm::vec2> quadTexCoord = { 
		glm::vec2(0.0f, 0.0f), 
		glm::vec2(1.0f, 0.0f), 
		glm::vec2(0.0f, 1.0f), 
		glm::vec2(1.0f, 1.0f) 
	};

	unsigned int quadTexCoordVbo;
	glEnableVertexAttribArray(2);
	glGenBuffers(1, &quadTexCoordVbo);
	glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordVbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), &quadTexCoord[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenTextures(1, &m_pimpl->m_texId);
	glBindTexture(GL_TEXTURE_2D, m_pimpl->m_texId);

	int width = (int)(m_pimpl->m_topRightPosition.x - m_pimpl->m_bottomLeftPosition.x);
	int height = (int)(m_pimpl->m_topRightPosition.y - m_pimpl->m_bottomLeftPosition.y);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	UTexture::enableTexturesClampToEdge();
	UTexture::enableLinearFilter();

	m_pimpl->m_fbo = std::make_unique<UFbo>();
	m_pimpl->m_fbo->bindTexture(0, UFbo::BIND_COLORTEXTURE, m_pimpl->m_texId);
	m_pimpl->m_fbo->bindRenderBuffer(1, UFbo::BIND_DEPTHBUFFER, width, height);

	bool isOk = m_pimpl->m_fbo->isOk();
	UFbo::disable();
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(2);
	if (!isOk) {
		std::cout << "[ERROR] Invalid FBO" << std::endl;
		return false;
	}
	return true;
}

void U2DQuad::activeAsBuffer() {
	m_pimpl->m_fbo->render();
}

void U2DQuad::disableAsBuffer() {
	UFbo::disable();
}

void U2DQuad::render() {
	glBindVertexArray(m_pimpl->m_vao);

	// Set a matrix for the left "eye":    
	glm::mat4 f = glm::mat4(1.0f);

	// Setup the passthrough shader:
	std::shared_ptr<UProgramShader> shader = Utopia::getInstance().getPassThroughProgamShader();
	shader->render();

	int ptProjLoc = shader->getParamLocation("projection");
	int ptMvLoc = shader->getParamLocation("modelview");
	int ptColorLoc = shader->getParamLocation("color");

	shader->setMatrix4(ptProjLoc, glm::ortho(0.0f, (float) m_pimpl->m_orthoWidth, 0.0f, (float) m_pimpl->m_orthoHeight, -1.0f, 1.0f));
	shader->setMatrix4(ptMvLoc, m_pimpl->m_positionMatrix);
	shader->setVec4(ptColorLoc, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));

	glBindTexture(GL_TEXTURE_2D, m_pimpl->m_texId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
}
