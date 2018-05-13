#include <gl\glew.h>
#include <gl\freeglut.h>
#include <cstdio>
#include "Anim1Shader.hpp"
#include "..\owner\GLShaderOwner.hpp"
#include "..\owner\GLShaderCompilerOwner.hpp"
#include "..\owner\GLAttachShaderOwner.hpp"

const char *Anim1Shader::VTSD_PATH = "shader/anim1_vtsd.vert";
const char *Anim1Shader::FRSD_PATH = "shader/anim1_frsd.frag";

Anim1Shader::Anim1Shader()
{
}

Anim1Shader::Anim1Shader(bool flag) {
	init();
}

void Anim1Shader::useProgram()
{
	program.use();
}

void Anim1Shader::unUseProgram()
{
	program.unUse();
}

int Anim1Shader::getICoord()
{
	return iCoord;
}

int Anim1Shader::getINormal()
{
	return iNormal;
}

int Anim1Shader::getIBIDs(int index)
{
	return iBIDs[index];
}

int Anim1Shader::getIBWeights(int index)
{
	return iBWeights[index];
}

int Anim1Shader::getVTrans()
{
	return vTrans;
}

int Anim1Shader::getProjViewModel()
{
	return projViewModel;
}

Anim1Shader Anim1Shader::create()
{
	return Anim1Shader(true);
}

void Anim1Shader::init()
{
	GLShaderCompilerOwner compilerOwner;
	GLShaderOwner vtsd(GL_VERTEX_SHADER);
	GLShaderOwner frsd(GL_FRAGMENT_SHADER);
	vtsd.compile(VTSD_PATH);
	frsd.compile(FRSD_PATH);
	program = GLProgramOwner(true);
	GLuint programHandler = program.get();
	GLAttachShaderOwner attacher(programHandler);
	attacher.attach(vtsd.get());
	attacher.attach(frsd.get());
	program.link();
	program.unUse();
	iCoord = glGetAttribLocation(
		programHandler, "iCoord");
	iNormal = glGetAttribLocation(
		programHandler, "iNormal");
	iBIDs[0] = glGetAttribLocation(
		programHandler, "iBIDs[0]");
	iBIDs[1] = glGetAttribLocation(
		programHandler, "iBIDs[1]");
	iBWeights[0] = glGetAttribLocation(
		programHandler, "iBWeights[0]");
	iBWeights[1] = glGetAttribLocation(
		programHandler, "iBWeights[1]");
	vTrans = glGetUniformLocation(
		programHandler, "vTrans"
	);
	projViewModel = glGetUniformLocation(
		programHandler, "projViewModel"
	);
}


