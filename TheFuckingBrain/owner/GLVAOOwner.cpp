#include "GLVAOOwner.hpp"
#include <gl\glew.h>
#include <gl\freeglut.h>
#include <iostream>

GLVAOOwner::GLVAOOwner(GLsizei num) {
	vaos.resize(num);
	glGenVertexArrays(num, vaos.data());
	flag = true;
	this->num = num;
}

GLVAOOwner::~GLVAOOwner() {
	if (flag) {
		std::cout << "VAO owner destroyed." << std::endl;
		glDeleteVertexArrays(num, vaos.data());
	}
}

const GLuint * GLVAOOwner::get() const
{
	return vaos.data();
}

GLuint GLVAOOwner::get(unsigned int index) const
{
	return vaos[index];
}

GLVAOOwner & GLVAOOwner::operator=(GLVAOOwner &another)
{
	if (this == &another) {
		return *this;
	}
	if (!another.flag || this->flag) {
		return *this;
	}
	this->flag = true;
	this->num = another.num;	
	this->vaos = another.vaos;
	another.flag = false;
	return *this;
}
