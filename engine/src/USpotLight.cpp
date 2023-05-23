#include "USpotLight.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "UProgramShader.h"
#include "UVertexShader.h"
#include "Utopia.h"
#include "UCamera.h"
#include "resource.h"
#include "UResources.h"

using namespace utopia;

static std::shared_ptr<UProgramShader> m_programShader;

struct USpotLight::pimpl {
	float m_cutoff;
	glm::vec3 m_direction;
	float m_constantAttenuation;
	float m_linearAttenuation;
	float m_quadraticAttenuation;


	pimpl(const float cutoff, const glm::vec3& direction, const float constantAttenuation, const float linearAttenuation, const float quadraticAttenuation)
	{
		this->m_cutoff = cutoff;
		this->m_direction = direction;
		this->m_constantAttenuation = constantAttenuation;
		this->m_linearAttenuation = linearAttenuation;
		this->m_quadraticAttenuation = quadraticAttenuation;
	}
};

USpotLight::~USpotLight() noexcept = default;

void USpotLight::render()
{
	UProgramShader::setActiveProgramShader(m_programShader);
	UProgramShader::getActiveProgramShader()->render();
	UProgramShader::getActiveProgramShader()->bind(0, "in_Position");
	UProgramShader::getActiveProgramShader()->bind(1, "in_Normal");

	ULight::render();

	/*
	glLightfv(getLightID(), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	glLightfv(getLightID(), GL_SPOT_CUTOFF, &m_pimpl->m_cutoff);
	glLightfv(getLightID(), GL_SPOT_DIRECTION, glm::value_ptr(m_pimpl->m_direction));
	glLightf(getLightID(), GL_CONSTANT_ATTENUATION, m_pimpl->m_constantAttenuation);
	glLightf(getLightID(), GL_LINEAR_ATTENUATION, m_pimpl->m_linearAttenuation);
	glLightf(getLightID(), GL_QUADRATIC_ATTENUATION, m_pimpl->m_quadraticAttenuation);
	*/


	auto modelViewEye = glm::inverse( UCamera::getMainCamera().lock()->getFinalWorldCoordinates()) * getModelView();
	auto direction = glm::vec3(modelViewEye * glm::vec4(m_pimpl->m_direction, 1.f));

	auto lightPosition = UProgramShader::getActiveProgramShader()->getParamLocation("lightPosition");

	glm::vec3 lightPos = glm::vec3(modelViewEye[3][0], modelViewEye[3][1], modelViewEye[3][2]);
	UProgramShader::getActiveProgramShader()->setVec3(lightPosition, lightPos);



	auto spotLightDirection = UProgramShader::getActiveProgramShader()->getParamLocation("spotLightDirection");
	UProgramShader::getActiveProgramShader()->setVec3(spotLightDirection, direction);


	auto spotLightCutoff = UProgramShader::getActiveProgramShader()->getParamLocation("spotLightCutoff");
	UProgramShader::getActiveProgramShader()->setFloat(spotLightCutoff, m_pimpl->m_cutoff);


	auto spotLightLinearAttenuation = UProgramShader::getActiveProgramShader()->getParamLocation("spotLightLinearAttenuation");
	UProgramShader::getActiveProgramShader()->setFloat(spotLightLinearAttenuation, m_pimpl->m_linearAttenuation);

	auto spotLightQuadraticAttenuation = UProgramShader::getActiveProgramShader()->getParamLocation("spotLightQuadraticAttenuation");
	UProgramShader::getActiveProgramShader()->setFloat(spotLightQuadraticAttenuation, m_pimpl->m_quadraticAttenuation);
}

USpotLight::USpotLight(const std::string& name, const float cutoff, const glm::vec3& direction, const float constantAttenuation, const float linearAttenuation, const float quadraticAttenuation) : ULight{ name }, m_pimpl{ std::unique_ptr<pimpl>(new pimpl(cutoff, direction, constantAttenuation, linearAttenuation, quadraticAttenuation)) } 
{
	if (m_programShader == nullptr)
	{
		m_programShader = std::shared_ptr<UProgramShader>(new UProgramShader("spotProg"));
		std::unique_ptr<UFragmentShader> fragShader = std::unique_ptr<UFragmentShader>(new UFragmentShader("spotFrag"));
		fragShader->loadFromMemory(*UResources::getInstance().getStringResource(IDR_FRAG_SPOT, FRAG));
		std::unique_ptr<UVertexShader> m_vertexShader = std::unique_ptr<UVertexShader>(new UVertexShader("vertSpot"));
		m_vertexShader->loadFromMemory(*UResources::getInstance().getStringResource(IDR_VERT_DEFAULT,VERT));

		m_programShader->build(*m_vertexShader, *fragShader);
	}
}

void USpotLight::setDirection(const glm::vec3& direction)
{
	m_pimpl->m_direction = direction;
}

auto USpotLight::getDirection() const -> const glm::vec3&
{
	return m_pimpl->m_direction;
}

void USpotLight::setCutoff(const float& cutoff)
{
	m_pimpl->m_cutoff = cutoff;
}

auto USpotLight::getCutoff() const -> const float&
{
	return m_pimpl->m_cutoff;
}

void USpotLight::setConstantAttenuation(const float& constantAttenuation)
{
	m_pimpl->m_constantAttenuation = constantAttenuation;
}

auto USpotLight::getConstantAttenuation() const -> const float&
{
	return m_pimpl->m_constantAttenuation;
}

void USpotLight::setLinearAttenuation(const float& linearAttenuation)
{
	m_pimpl->m_linearAttenuation = linearAttenuation;
}

auto USpotLight::getLinearAttenuation() const -> const float&
{
	return m_pimpl->m_linearAttenuation;
}

void USpotLight::setQuadraticAttenuation(const float& quadraticAttenuation)
{
	m_pimpl->m_quadraticAttenuation = quadraticAttenuation;
}

auto USpotLight::getQuadraticAttenuation() const -> const float&
{
	return m_pimpl->m_quadraticAttenuation;
}
