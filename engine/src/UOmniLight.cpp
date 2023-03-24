#include "UOmniLight.h"
#include "UVertexShader.h"
#include "UCamera.h"
#include "Utopia.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


using namespace utopia;

static std::shared_ptr<UProgramShader> m_programShader;

struct UOmniLight::pimpl {
	float m_constantAttenuation;
	float m_linearAttenuation;
	float m_quadraticAttenuation;


	pimpl(const float constantAttenuation = 1, const float linearAttenuation = 0, const float quadraticAttenuation = 0)
	{
		this->m_constantAttenuation = constantAttenuation;
		this->m_linearAttenuation = linearAttenuation;
		this->m_quadraticAttenuation = quadraticAttenuation;
	}
};

UOmniLight::~UOmniLight() noexcept = default;

void UOmniLight::render()
{
	UProgramShader::setActiveProgramShader(m_programShader);
	UProgramShader::getActiveProgramShader()->render();
	UProgramShader::getActiveProgramShader()->bind(0, "in_Position");
	UProgramShader::getActiveProgramShader()->bind(1, "in_Normal");

	auto projLoc = UProgramShader::getActiveProgramShader()->getParamLocation("projection");
	UProgramShader::getActiveProgramShader()->setMatrix4(projLoc, UCamera::getMainCamera().lock()->getCameraMatrix());

	ULight::render();
}

UOmniLight::UOmniLight(const std::string& name, const float constantAttenuation, const float linearAttenuation, const float quadraticAttenuation) : ULight{ name }, m_pimpl{ std::unique_ptr<pimpl>(new pimpl(constantAttenuation, linearAttenuation, quadraticAttenuation)) } 
{
	if (m_programShader == nullptr)
	{
		m_programShader = std::shared_ptr<UProgramShader>(new UProgramShader("progOmni"));

		std::unique_ptr<UFragmentShader> m_fragmentShader = std::unique_ptr<UFragmentShader>(new UFragmentShader("fragOmni"));
		m_fragmentShader->loadFromFile("shaders/omniShader.frag");

		std::unique_ptr<UVertexShader> m_vertexShader = std::unique_ptr<UVertexShader>(new UVertexShader("vertOmni"));
		m_vertexShader->loadFromFile("shaders/defaultShader.vert");
		

		m_programShader->build(*m_vertexShader, *m_fragmentShader);
	}
}

void UOmniLight::setConstantAttenuation(const float& constantAttenuation)
{
	m_pimpl->m_constantAttenuation = constantAttenuation;
}

auto UOmniLight::getConstantAttenuation() const -> const float&
{
	return m_pimpl->m_constantAttenuation;
}

void UOmniLight::setLinearAttenuation(const float& linearAttenuation)
{
	m_pimpl->m_linearAttenuation = linearAttenuation;
}

auto UOmniLight::getLinearAttenuation() const -> const float&
{
	return m_pimpl->m_linearAttenuation;
}

void UOmniLight::setQuadraticAttenuation(const float& quadraticAttenuation)
{
	m_pimpl->m_quadraticAttenuation = quadraticAttenuation;
}

auto UOmniLight::getQuadraticAttenuation() const -> const float&
{
	return m_pimpl->m_quadraticAttenuation;
}
