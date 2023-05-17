#include "ULight.h"
#include "UCamera.h"
#include "Utopia.h"
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include "UProgramShader.h"
#include <glm/gtx/string_cast.hpp>


using namespace utopia;

struct ULight::pimpl {
	
	glm::vec4 m_ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	glm::vec4 m_diffuse = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
	glm::vec4 m_specular = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
	glm::vec4 m_gAmbient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	
	pimpl() {}
};

unsigned int ULight::m_nLightsUsed{ 0 };

int utopia::ULight::getnLightsUsed()
{
	return m_nLightsUsed;
}

ULight::ULight(const std::string& name) : m_pimpl{ std::unique_ptr<pimpl>(new pimpl())}, UNode { name }
{
	m_nLightsUsed++;
}
ULight::~ULight() noexcept
{
	m_nLightsUsed--;
}

void ULight::render()
{
	//render something somehow and then we make a call to UNode render, to render all of the children
		
	UNode::render();

	auto projLoc = UProgramShader::getActiveProgramShader()->getParamLocation("projection");
	UProgramShader::getActiveProgramShader()->setMatrix4(projLoc, UCamera::getMainCamera().lock()->getCameraMatrix());

	auto nLightUsed = UProgramShader::getActiveProgramShader()->getParamLocation("nLightUsed");
	UProgramShader::getActiveProgramShader()->setInt(nLightUsed, ULight::getnLightsUsed());

	auto lightAmbient = UProgramShader::getActiveProgramShader()->getParamLocation("lightAmbient");
	UProgramShader::getActiveProgramShader()->setVec3(lightAmbient, glm::vec3(getAmbient()));

	auto lightDiffuse = UProgramShader::getActiveProgramShader()->getParamLocation("lightDiffuse");
	UProgramShader::getActiveProgramShader()->setVec3(lightDiffuse, glm::vec3(getDiffuse()));

	auto lightSpecular = UProgramShader::getActiveProgramShader()->getParamLocation("lightSpecular");
	UProgramShader::getActiveProgramShader()->setVec3(lightSpecular, glm::vec3(getSpecular()));

	/*
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(m_pimpl->m_gAmbient));

	auto modelViewEye = glm::inverse(UCamera::getMainCamera().lock()->getFinalWorldCoordinates())* getModelView();
	auto lightPosition = UProgramShader::getActiveProgramShader()->getParamLocation("lightPosition");
	UProgramShader::getActiveProgramShader()->setVec3(lightPosition, glm::vec3(modelViewEye[3][0], modelViewEye[3][1], modelViewEye[3][2]));

	auto lightAmbient = UProgramShader::getActiveProgramShader()->getParamLocation("lightAmbient");
	UProgramShader::getActiveProgramShader()->setVec3(lightAmbient,glm::vec3(m_pimpl->m_ambient));

	auto lightDiffuse = UProgramShader::getActiveProgramShader()->getParamLocation("lightDiffuse");
	UProgramShader::getActiveProgramShader()->setVec3(lightDiffuse,glm::vec3(m_pimpl->m_diffuse));

	auto lightSpecular = UProgramShader::getActiveProgramShader()->getParamLocation("lightSpecular");
	UProgramShader::getActiveProgramShader()->setVec3(lightSpecular,glm::vec3(m_pimpl->m_specular));
	*/
	/*
	auto lightAmbient = Utopia::getInstance().getBasicProgramShader()->getParamLocation("lightAmbient");
	Utopia::getInstance().getBasicProgramShader()->setVec3(lightAmbient,glm::vec3(1.f,1.f,1.f));

	auto lightDiffuse = Utopia::getInstance().getBasicProgramShader()->getParamLocation("lightDiffuse");
	Utopia::getInstance().getBasicProgramShader()->setVec3(lightDiffuse, glm::vec3(1.f, 1.f, 1.f));

	auto lightSpecular = Utopia::getInstance().getBasicProgramShader()->getParamLocation("lightSpecular");
	Utopia::getInstance().getBasicProgramShader()->setVec3(lightSpecular,glm::vec3(glm::vec3(1.f, 1.f, 1.f)));
	*/

	//glLightfv(getLightID(), GL_AMBIENT, glm::value_ptr(m_pimpl->m_ambient));
	//glLightfv(getLightID(), GL_DIFFUSE, glm::value_ptr(m_pimpl->m_diffuse));
	//glLightfv(getLightID(), GL_SPECULAR, glm::value_ptr(m_pimpl->m_specular));

	//glEnable(getLightID());
}

void ULight::setAmbient(const glm::vec4& ambient)
{
	m_pimpl->m_ambient = ambient;
}

void ULight::setDiffuse(const glm::vec4& diffuse)
{
	m_pimpl->m_diffuse = diffuse;
}

void ULight::setSpecular(const glm::vec4& specular)
{
	m_pimpl->m_specular = specular;
}

auto ULight::getAmbient() const -> const glm::vec4&
{
	return m_pimpl->m_ambient;
}

auto ULight::getDiffuse() const -> const glm::vec4&
{
	return m_pimpl->m_diffuse;
}

auto ULight::getSpecular() const -> const glm::vec4&
{
	return m_pimpl->m_specular;
}

void ULight::setGlobalAmbient(const glm::vec4& gAmbient)
{
	m_pimpl->m_gAmbient = gAmbient;
}

auto ULight::getGlobalAmbient() const -> const glm::vec4&
{
	return m_pimpl->m_gAmbient;
}