#include <gl\glew.h>
#include <gl\freeglut.h>
#include <iostream>
#include "GLShaderCompilerOwner.hpp"

GLShaderCompilerOwner::~GLShaderCompilerOwner() {
	std::cout << "compiler destroyed." << std::endl;
	glReleaseShaderCompiler();
}