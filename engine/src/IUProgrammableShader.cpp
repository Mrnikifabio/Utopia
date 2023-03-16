#include "IUProgrammableShader.h"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#define MAX_LOGSIZE 4096

using namespace utopia;

bool IUProgrammableShader::loadFromFile(const std::string& filename)
{
	// Open file:
	FILE* dat = fopen(filename.c_str(), "rt");
	if (dat == nullptr)
	{
		std::cout << "[ERROR] Unable to open file" << std::endl;
		return false;
	}

	// Get file length:
	unsigned int length;
	fseek(dat, 0L, SEEK_END);
	length = ftell(dat);
	fseek(dat, 0L, SEEK_SET);

	// Read code:
	char* content = nullptr;
	if (length > 0)
	{
		content = new char[sizeof(char) * (length + 1)];
		length = (unsigned int)fread(content, sizeof(char), length, dat);
		content[length] = '\0';
	}
	fclose(dat);

	// Load from memory then:
	bool result = loadFromMemory(content);

	// Clean:
	delete[] content;

	// Done:
	return result;
}

bool IUProgrammableShader::commonLoad(unsigned int& glId, int glKind, const std::string& data)
{
	glId = glCreateShader(glKind);
	if (glId == 0)
	{
		std::cout << "[ERROR] Unable to create shader object" << std::endl;
		return false;
	}

	GLchar const* files[] = { data.c_str() };
	glShaderSource(glId, 1, files, NULL);
	glCompileShader(glId);

	// Verify shader:
	int status;
	char buffer[MAX_LOGSIZE];
	int length = 0;
	memset(buffer, 0, MAX_LOGSIZE);

	glGetShaderiv(glId, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(glId, MAX_LOGSIZE, &length, buffer);
	if (status == false)
	{
		std::cout << "[ERROR] Shader not compiled: " << buffer << std::endl;
		return false;
	}

	// Done:
	return true;
}
