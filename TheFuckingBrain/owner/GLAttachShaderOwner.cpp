#include "GLAttachShaderOwner.hpp"
#include <gl\glew.h>
#include <GL\freeglut.h>
#include <vector>
#include <iostream>
#include <exception>


GLAttachShaderOwner::GLAttachShaderOwner(
	GLuint handler) {
	if (glIsProgram(handler) == GL_FALSE) {
		throw std::exception("GLAttachShaderOwner constructor exception.");
	}
	this->handler = handler;
}

GLAttachShaderOwner::~GLAttachShaderOwner() {
	typedef std::vector<GLuint>::iterator sIterator;
	sIterator begin = shaders.begin();
	sIterator end = shaders.end();
	for (sIterator index = begin; index != end; index++) {
		std::cout << "Detach Shader." << std::endl;
		glDetachShader(handler, *index);
	}
}

void GLAttachShaderOwner::attach(GLuint shader) {
	if (glIsShader(shader) == GL_FALSE) {
		return;
	}
	glAttachShader(handler, shader);
	shaders.push_back(shader);
}