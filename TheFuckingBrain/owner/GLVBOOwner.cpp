#include "GLVBOOwner.hpp"
#include <gl\glew.h>
#include <GL\freeglut.h>
#include <iostream>

GLVBOOwner::GLVBOOwner(GLsizei num) {
	vbos.resize(num);
	glGenBuffers(num, vbos.data());
	this->num = num;
	flag = true;
}

GLVBOOwner::~GLVBOOwner() {
	if (flag) {
		std::cout << "VBO owner destroyed." << std::endl;
		glDeleteBuffers(num, vbos.data());
	}
}

const GLuint * GLVBOOwner::get() const
{
	return vbos.data();
}

GLuint GLVBOOwner::get(unsigned int index) const
{
	return vbos[index];
}

GLVBOOwner & GLVBOOwner::operator=(GLVBOOwner & another)
{
	if (!another.flag || this->flag) {
		return *this;
	}
	this->flag = true;
	this->num = another.num;
	this->vbos = another.vbos;
	another.flag = false;
	return *this;
}

