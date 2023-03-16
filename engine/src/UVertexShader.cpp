#include "UVertexShader.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace utopia;

UVertexShader::UVertexShader(const std::string& name) : UShader{ name } {}

UVertexShader::~UVertexShader() noexcept
{
	if(getID())
		glDeleteShader(getID());
}



void UVertexShader::render()
{
	//does nothing...
}

bool UVertexShader::loadFromMemory(const std::string& data)
{
	int glKind = GL_VERTEX_SHADER;
	unsigned int glID = getID();
	if (glID)
		glDeleteShader(glID);

	auto result = commonLoad(glID, glKind, data);
	setID(glID);
	return result;
}
