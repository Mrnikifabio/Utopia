#include "UShader.h"
#include <GL/glew.h>
#include <GL/freeglut.h>


using namespace utopia;

UShader::UShader(const std::string& name) : glId{ 0 }, UObject{ name } {}

UShader::~UShader() = default;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Returns the param location given its variable name.
 * @param name variable name
 * @return location ID or -1 if not found
 */
int UShader::getParamLocation(const std::string& name) const
{
	// Return location:
	int r = glGetUniformLocation(glId, name.c_str());
	if (r == -1)
		std::cout << "[ERROR] Param '" << name << "' not found" << std::endl;
	return r;
}

void UShader::render()
{
	//does nothing
}

int UShader::getID() const
{
	return glId;
}

void UShader::setID(unsigned int id)
{
	glId = id;
}

void UShader::setMatrix4(int param, const glm::mat4& mat)
{
	glUniformMatrix4fv(param, 1, GL_FALSE, glm::value_ptr(mat));
}

void UShader::setMatrix3(int param, const glm::mat3& mat)
{
	glUniformMatrix3fv(param,1, GL_FALSE, glm::value_ptr(mat));
}

void UShader::setFloat(int param, float value)
{
	glUniform1f(param, value);
}

void UShader::setInt(int param, int value)
{
	glUniform1i(param, value);
}

void UShader::setVec3(int param, const glm::vec3& vect)
{
	glUniform3fv(param, 1, glm::value_ptr(vect));
}

void UShader::setVec4(int param, const glm::vec4& vect)
{
	glUniform4fv(param, 1, glm::value_ptr(vect));
}

void UShader::bind(int location, const char* attribName)
{
	glBindAttribLocation(glId, location, attribName);
}
