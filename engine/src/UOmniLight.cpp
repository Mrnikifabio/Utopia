#include "UOmniLight.h"
#include "UVertexShader.h"
#include "UCamera.h"
#include "Utopia.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <resource.h>
#include "UResources.h"


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

	auto modelViewEye = glm::inverse(UCamera::getMainCamera().lock()->getFinalWorldCoordinates()) * getModelView();
	auto lightPosition = UProgramShader::getActiveProgramShader()->getParamLocation("lightPosition");

	glm::vec3 lightPos = glm::vec3(modelViewEye[3][0], modelViewEye[3][1], modelViewEye[3][2]);
	UProgramShader::getActiveProgramShader()->setVec3(lightPosition, lightPos);



	ULight::render();
}

UOmniLight::UOmniLight(const std::string& name, const float constantAttenuation, const float linearAttenuation, const float quadraticAttenuation) : ULight{ name }, m_pimpl{ std::unique_ptr<pimpl>(new pimpl(constantAttenuation, linearAttenuation, quadraticAttenuation)) } 
{
	if (m_programShader == nullptr)
	{
		m_programShader = std::shared_ptr<UProgramShader>(new UProgramShader("progOmni"));

		std::unique_ptr<UFragmentShader> m_fragmentShader = std::unique_ptr<UFragmentShader>(new UFragmentShader("fragOmni"));
		m_fragmentShader->loadFromMemory(UResources::getInstance().getStringResource(IDR_FRAG_OMNI, FRAG));

		std::unique_ptr<UVertexShader> m_vertexShader = std::unique_ptr<UVertexShader>(new UVertexShader("vertOmni"));
		m_vertexShader->loadFromMemory(UResources::getInstance().getStringResource(IDR_VERT_DEFAULT, VERT));
		

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
