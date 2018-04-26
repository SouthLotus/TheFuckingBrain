#include "GLTextureOwner.hpp"
#include <gl\glew.h>
#include <iostream>

GLTextureOwner::GLTextureOwner(GLsizei num) {
	texs.resize(num);
	glGenTextures(num, texs.data());
	this->num = num;
	flag = true;
}

GLTextureOwner::~GLTextureOwner() {
	if (flag) {
		std::cout << "Texture owner destroyed." << std::endl;
		glDeleteTextures(num, texs.data());
	}
}

const GLuint * GLTextureOwner::get() const
{
	return texs.data();
}

GLuint GLTextureOwner::get(unsigned int index) const
{
	return texs[index];
}

GLTextureOwner & GLTextureOwner::operator=(GLTextureOwner &another)
{
	if (this == &another) {
		return *this;
	}
	if (!another.flag || this->flag) {
		return *this;
	}
	this->flag = true;
	this->num = another.num;
	this->texs = another.texs;
	another.flag = false;
	return *this;
}
