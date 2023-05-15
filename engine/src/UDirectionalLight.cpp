#include "UDirectionalLight.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <UCamera.h>
#include <UProgramShader.h>


using namespace utopia;

static std::shared_ptr<UProgramShader> m_programShader;

UDirectionalLight::~UDirectionalLight() noexcept = default;

UDirectionalLight::UDirectionalLight(const std::string& name) : ULight {name} {
	if (m_programShader == nullptr)
	{
		m_programShader = std::shared_ptr<UProgramShader>(new UProgramShader("progOmni"));

		std::unique_ptr<UFragmentShader> m_fragmentShader = std::unique_ptr<UFragmentShader>(new UFragmentShader("fragOmni"));
		m_fragmentShader->loadFromFile("shaders/directionalShader.frag");

		std::unique_ptr<UVertexShader> m_vertexShader = std::unique_ptr<UVertexShader>(new UVertexShader("vertOmni"));
		m_vertexShader->loadFromFile("shaders/defaultShader.vert");

		m_programShader->build(*m_vertexShader, *m_fragmentShader);
	}
}

void UDirectionalLight::render()
{

	UProgramShader::setActiveProgramShader(m_programShader);
	UProgramShader::getActiveProgramShader()->render();
	UProgramShader::getActiveProgramShader()->bind(0, "in_Position");
	UProgramShader::getActiveProgramShader()->bind(1, "in_Normal");

	ULight::render();
}