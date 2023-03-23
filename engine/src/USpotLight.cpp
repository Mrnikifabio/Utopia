#include "USpotLight.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include "UProgramShader.h"
#include "UVertexShader.h"
#include "Utopia.h"
#include "UCamera.h"


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

	auto projLoc = UProgramShader::getActiveProgramShader()->getParamLocation("projection");
	UProgramShader::getActiveProgramShader()->setMatrix4(projLoc, UCamera::getMainCamera().lock()->getCameraMatrix());

	ULight::render();

	/*
	glLightfv(getLightID(), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	glLightfv(getLightID(), GL_SPOT_CUTOFF, &m_pimpl->m_cutoff);
	glLightfv(getLightID(), GL_SPOT_DIRECTION, glm::value_ptr(m_pimpl->m_direction));
	glLightf(getLightID(), GL_CONSTANT_ATTENUATION, m_pimpl->m_constantAttenuation);
	glLightf(getLightID(), GL_LINEAR_ATTENUATION, m_pimpl->m_linearAttenuation);
	glLightf(getLightID(), GL_QUADRATIC_ATTENUATION, m_pimpl->m_quadraticAttenuation);
	*/

	auto modelViewEye = glm::inverse(UCamera::getMainCamera().lock()->getFinalWorldCoordinates()) * getModelView();
	auto lightPosition = UProgramShader::getActiveProgramShader()->getParamLocation("spotLightPosition");

	UProgramShader::getActiveProgramShader()->setVec3(lightPosition, glm::vec3(modelViewEye[3][0], modelViewEye[3][1], modelViewEye[3][2]));

	auto lightAmbient = UProgramShader::getActiveProgramShader()->getParamLocation("spotLightAmbient");
	UProgramShader::getActiveProgramShader()->setVec3(lightAmbient, glm::vec3(getAmbient()));

	auto lightDiffuse = UProgramShader::getActiveProgramShader()->getParamLocation("spotLightDiffuse");
	UProgramShader::getActiveProgramShader()->setVec3(lightDiffuse, glm::vec3(getDiffuse()));

	auto lightSpecular = UProgramShader::getActiveProgramShader()->getParamLocation("spotLightSpecular");
	UProgramShader::getActiveProgramShader()->setVec3(lightSpecular, glm::vec3(getSpecular()));


	auto spotLightDirection = UProgramShader::getActiveProgramShader()->getParamLocation("spotLightDirection");
	UProgramShader::getActiveProgramShader()->setVec3(spotLightDirection, glm::vec3(m_pimpl->m_direction));

	auto spotLightCutoff = UProgramShader::getActiveProgramShader()->getParamLocation("spotLightCutoff");
	UProgramShader::getActiveProgramShader()->setFloat(spotLightCutoff, m_pimpl->m_cutoff);

	auto spotLightAttenuation = UProgramShader::getActiveProgramShader()->getParamLocation("spotLightDirection");
	UProgramShader::getActiveProgramShader()->setFloat(spotLightAttenuation,m_pimpl->m_linearAttenuation);
}

USpotLight::USpotLight(const std::string& name, const float cutoff, const glm::vec3& direction, const float constantAttenuation, const float linearAttenuation, const float quadraticAttenuation) : ULight{ name }, m_pimpl{ std::unique_ptr<pimpl>(new pimpl(cutoff, direction, constantAttenuation, linearAttenuation, quadraticAttenuation)) } 
{
	if (m_programShader == nullptr)
	{
		m_programShader = std::shared_ptr<UProgramShader>(new UProgramShader("spotProg"));
		std::unique_ptr<UFragmentShader> fragShader = std::unique_ptr<UFragmentShader>(new UFragmentShader("spotFrag"));
		fragShader->loadFromFile("shaders/spotShader.frag");

		std::unique_ptr<UVertexShader> m_vertexShader = std::unique_ptr<UVertexShader>(new UVertexShader("vertSpot"));
		m_vertexShader->loadFromFile("shaders/defaultShader.vert");

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
