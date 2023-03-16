#include "UFragmentShader.h"
#include <GL/glew.h>
#include <GL/freeglut.h>


using namespace utopia;

UFragmentShader::UFragmentShader(const std::string& name) : UShader{ name } {}

UFragmentShader::~UFragmentShader() noexcept
{
	if (getID())
		glDeleteShader(getID());
}

void UFragmentShader::render()
{
	//does nothing...
}

bool UFragmentShader::loadFromMemory(const std::string& data)
{
	int glKind = GL_FRAGMENT_SHADER;
	unsigned int glID = getID();
	if (glID)
		glDeleteShader(glID);

	auto result = commonLoad(glID, glKind, data);
	setID(glID);
	return result;
}