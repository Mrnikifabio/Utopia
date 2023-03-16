#include "UProgramShader.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace utopia;

#define MAX_LOGSIZE 4096


UProgramShader::UProgramShader(const std::string& name) : built{false}, UShader{name} {}

UProgramShader::~UProgramShader() noexcept
{
	int glId = getID();
	if (glId)
		glDeleteProgram(glId);
}

void UProgramShader::render()
{
	int glId = getID();

	// Activate shader:
	if (glId)
		glUseProgram(glId);
	else
	{
		std::cout << "[ERROR] Invalid shader rendered" << std::endl;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Loads and compiles a full program.
 * @param vertexShader pointer to a vertex shader, or nullptr if not used
 * @param fragmentShader pointer to a fragment shader, or nullptr if not used
 * @return true/false on success/failure
 */
bool UProgramShader::build(const UVertexShader& vertexShader, const UFragmentShader& fragmentShader)
{
	int glId = getID();

	// Delete if already used:
	if (glId)
	{
		// On reload, make sure it was a program before:
		if (this->built)
		{
			std::cout << "[ERROR] Cannot reload a shader as a program" << std::endl;
			return false;
		}
		glDeleteProgram(glId);
	}

	// Create program:
	glId = glCreateProgram();
	setID(glId);
	if (glId == 0)
	{
		std::cout << "[ERROR] Unable to create program" << std::endl;
		return false;
	}

	// Bind vertex shader:
	glAttachShader(glId, vertexShader.getID());

	// Bind fragment shader:
	glAttachShader(glId, fragmentShader.getID());

	// Link program:
	glLinkProgram(glId);
	this->built = true;

	// Verify program:
	int status;
	char buffer[MAX_LOGSIZE];
	int length = 0;
	memset(buffer, 0, MAX_LOGSIZE);

	glGetProgramiv(glId, GL_LINK_STATUS, &status);
	glGetProgramInfoLog(glId, MAX_LOGSIZE, &length, buffer);
	if (status == false)
	{
		std::cout << "[ERROR] Program link error: " << buffer << std::endl;
		return false;
	}
	glValidateProgram(glId);
	glGetProgramiv(glId, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cout << "[ERROR] Unable to validate program" << std::endl;
		return false;
	}

	// Done:
	return true;
}

bool UProgramShader::isBuilt()
{
	return built;
}
