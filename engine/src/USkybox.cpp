#include "USkybox.h"
#include "UCamera.h"
#include "Utopia.h"

#include <gl/glew.h>
#include <gl/freeglut.h>
#include "UResources.h"
#include <resource.h>



using namespace utopia;

struct USkybox::pimpl {
    std::shared_ptr<UCubeMapTexture> m_texture = nullptr;
    unsigned int cubeVboVertices = 0;
    unsigned int cubeVboFaces = 0;
    unsigned int m_vao = 0;
    bool vaoLoaded = false;
};

std::shared_ptr<UVertexShader> USkybox::m_vert = nullptr;
std::shared_ptr<UFragmentShader> USkybox::m_frag = nullptr;
std::shared_ptr<UProgramShader> USkybox::m_prog = nullptr;

float USkybox::cubeVertices[] = {
      -1.0f,  1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
};
unsigned short USkybox::cubeFaces[] = {
    0, 1, 2,
    0, 2, 3,
    3, 2, 6,
    3, 6, 7,
    4, 0, 3,
    4, 3, 7,
    6, 5, 4,
    7, 6, 4,
    4, 5, 1,
    4, 1, 0,
    1, 5, 6,
    1, 6, 2,
};

USkybox::USkybox(const std::string& name) : UNode{ name }, m_pimpl{ std::unique_ptr<USkybox::pimpl>(new pimpl()) }
{
    if (m_vert == nullptr)
    {
        m_vert = std::shared_ptr<UVertexShader>(new UVertexShader("skybox_vertex_shader"));
        m_vert->loadFromMemory(UResources::getInstance().getStringResource(IDR_VERT_SKYBOX, VERT));
    }

    if (m_frag == nullptr)
    {
        m_frag = std::shared_ptr<UFragmentShader>(new UFragmentShader("skybox_frag_shader"));
        m_frag->loadFromMemory(UResources::getInstance().getStringResource(IDR_FRAG_SKYBOX, FRAG));
            
    }

    if (m_prog == nullptr)
    {
        m_prog = std::shared_ptr<UProgramShader>(new UProgramShader("skybox_prog_shader"));
        m_prog->build(*m_vert.get(), *m_frag.get());
    }
}

void USkybox::setTexture(std::shared_ptr<UCubeMapTexture> texture)
{
    this->m_pimpl->m_texture = texture;
}

USkybox::~USkybox()
{
    glDeleteBuffers(1, &this->m_pimpl->cubeVboVertices);
    glDeleteBuffers(1, &this->m_pimpl->cubeVboFaces);
    glDeleteVertexArrays(1, &this->m_pimpl->m_vao);
}

void USkybox::render()
{
   auto old = UProgramShader::getActiveProgramShader();

    UProgramShader::setActiveProgramShader(m_prog);
    UProgramShader::getActiveProgramShader()->render();

    auto m = glm::inverse(UCamera::getMainCamera().lock()->getFinalWorldCoordinates()) * getModelView();
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;

    auto modelView = UProgramShader::getActiveProgramShader()->getParamLocation("modelview");

    UProgramShader::getActiveProgramShader()->setMatrix4(modelView, m);

    auto projLoc = UProgramShader::getActiveProgramShader()->getParamLocation("projection");
    UProgramShader::getActiveProgramShader()->setMatrix4(projLoc, UCamera::getMainCamera().lock()->getCameraMatrix());


    if (!this->m_pimpl->vaoLoaded)
    {
        // Init VAO:
        glGenVertexArrays(1, &this->m_pimpl->m_vao);
        glBindVertexArray(this->m_pimpl->m_vao);

        // Load cube data into a VBO:
        glGenBuffers(1, &this->m_pimpl->cubeVboVertices);
        glBindBuffer(GL_ARRAY_BUFFER, this->m_pimpl->cubeVboVertices);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &this->m_pimpl->cubeVboFaces);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_pimpl->cubeVboFaces);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeFaces), cubeFaces, GL_STATIC_DRAW);

        // Unbind the VAO
        glBindVertexArray(0);
        this->m_pimpl->vaoLoaded = true;
    }

    glBindVertexArray(this->m_pimpl->m_vao);
    m_prog->bind(0, "in_Position");

    if (this->m_pimpl->m_texture != nullptr)
        this->m_pimpl->m_texture->render();

    glDisable(GL_CULL_FACE);

    glDrawElements(GL_TRIANGLES, sizeof(cubeFaces) / sizeof(unsigned short), GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);

    glEnable(GL_CULL_FACE);

    UProgramShader::setActiveProgramShader(old);
    UProgramShader::getActiveProgramShader()->render();
}

