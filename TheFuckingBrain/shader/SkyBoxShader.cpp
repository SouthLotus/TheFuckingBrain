#include <gl\glew.h>
#include <gl\freeglut.h>
#include "SkyBoxShader.hpp"
#include "..\tool\ShaderTool.hpp"
#include <iostream>
#include <exception>
#include <string>
#include "..\owner\GLProgramOwner.hpp"
#include "..\owner\GLShaderCompilerOwner.hpp"
#include "..\owner\GLShaderOwner.hpp"
#include "..\owner\GLAttachShaderOwner.hpp"

const char *SkyBoxShader::VTSD_PATH = "shader/skybox_vtsd.vert";
const char *SkyBoxShader::FRSD_PATH = "shader/skybox_frsd.frag";

void SkyBoxShader::useProgram()
{
	programOwner.use();
}

void SkyBoxShader::unUseProgram()
{
	programOwner.unUse();
}

int SkyBoxShader::getIVCoord() const
{
	return iVCoord;
}

int SkyBoxShader::getUProjModelView() const
{
	return uProjModelView;
}

int SkyBoxShader::getUSampler() const
{
	return uSampler;
}

void SkyBoxShader::init() {
	GLShaderCompilerOwner compilerOwner;
	GLShaderOwner vtsdOwner(GL_VERTEX_SHADER);
	GLShaderOwner frsdOwner(GL_FRAGMENT_SHADER);
	GLuint vtsd = vtsdOwner.get();
	GLuint frsd = frsdOwner.get();
	vtsdOwner.compile(VTSD_PATH);
	frsdOwner.compile(FRSD_PATH);
	programOwner = GLProgramOwner(true);
	GLuint programHolder = programOwner.get();
	GLAttachShaderOwner attachOwner(programHolder);
	attachOwner.attach(vtsd);
	attachOwner.attach(frsd);
	programOwner.link();
	programOwner.unUse();
	iVCoord = glGetAttribLocation(
		programHolder, "iVCoord");
	uProjModelView = glGetUniformLocation(
		programHolder, "projViewModelMat");
	uSampler = glGetUniformLocation(
		programHolder, "sampler"
	);
}