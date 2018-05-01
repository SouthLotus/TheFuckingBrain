#include "GLProgramOwner.hpp"
#include <gl\glew.h>
#include <gl\freeglut.h>
#include <exception>
#include <iostream>
#include "..\tool\ShaderTool.hpp"

GLProgramOwner::GLProgramOwner(bool flag) {
	if(!flag) {
		return;
	}
	handler = glCreateProgram();
	if (glIsProgram(handler) == GL_FALSE) {
		throw std::exception("GLProgramOwner constructor exceotion.");
	}
	this->flag = true;
}

GLProgramOwner::~GLProgramOwner() {
	if(flag) {
		std::cout << "program destroyed" << std::endl;
		glDeleteProgram(handler);
	}
}

GLuint GLProgramOwner::get() {
	return handler;
}

void GLProgramOwner::link()
{
	glLinkProgram(handler);
	std::string programStatus =
		ShaderTool::checkProgramStatus(handler);
	if (programStatus != "") {
		throw std::runtime_error(programStatus);
	}
}

void GLProgramOwner::use()
{
	glUseProgram(handler);
}

void GLProgramOwner::unUse()
{
	glUseProgram(0);
}

GLProgramOwner & GLProgramOwner::operator=(
	GLProgramOwner &another) {
	if (this == &another) {
		return *this;
	}
	if(!another.flag || this->flag) {
		return *this;
	}
	this->flag = true;
	this->handler = another.handler;
	another.flag = false;
	return *this;
}