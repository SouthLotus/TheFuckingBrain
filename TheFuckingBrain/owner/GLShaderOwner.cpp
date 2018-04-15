#include "GLShaderOwner.hpp"
#include <gl\glew.h>
#include <gl\freeglut.h>
#include <exception>
#include <iostream>
#include "..\tool\ShaderTool.hpp"

GLShaderOwner::GLShaderOwner(int type) {
	handler = glCreateShader(type);
	if (glIsShader(handler) == GL_FALSE) {
		throw std::exception("GLShaderOwner constructor exception.");
	}
}

GLShaderOwner::~GLShaderOwner() {
	std::cout << "shader destroyed." << std::endl;
	glDeleteShader(handler);
}

void GLShaderOwner::compile(const char * codeFile)
{
	std::string shaderCode =
		ShaderTool::loadShaderCode(codeFile);
	const char *shaderCodeCstr[] = { shaderCode.c_str() };
	glShaderSource(handler, 1, shaderCodeCstr, NULL);
	glCompileShader(handler);
	std::string shaderStatus =
		ShaderTool::checkShaderStatus(handler);
	if (shaderStatus != "") {
		throw std::runtime_error(shaderStatus);
	}
}

GLuint GLShaderOwner::get() {
	return handler;
}