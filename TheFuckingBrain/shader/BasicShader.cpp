#include <gl\glew.h>
#include <gl\freeglut.h>
#include "BasicShader.hpp"
#include "..\tool\ShaderTool.hpp"
#include <iostream>
#include <exception>
#include <string>
#include "..\owner\GLProgramOwner.hpp"
#include "..\owner\GLShaderCompilerOwner.hpp"
#include "..\owner\GLShaderOwner.hpp"
#include "..\owner\GLAttachShaderOwner.hpp"

const char *BasicShader::VTSD_PATH = "shader/vertex_shader.vert";
const char *BasicShader::FRSD_PATH = "shader/fragment_shader.frag";

void BasicShader::useProgram()
{
	programOwner.use();
}

void BasicShader::unUseProgram()
{
	programOwner.unUse();
}

int BasicShader::getIVCoord() const
{
	return iVCoord;
}

int BasicShader::getIVNormal() const
{
	return iVNormal;
}

int BasicShader::getIVTexCoord() const
{
	return iVTexCoord;
}

int BasicShader::getUProjModelView() const
{
	return uProjModelView;
}

int BasicShader::getUSampler() const
{
	return uSampler;
}

int BasicShader::getULitColor() const
{
	return uLitColor;
}

int BasicShader::getULitDirection() const
{
	return uLitDirection;
}

int BasicShader::getULitIntensity() const
{
	return uLitIntensity;
}

int BasicShader::getUCameraPosition() const
{
	return uCameraPosition;
}

void BasicShader::init() {
	GLShaderCompilerOwner compilerOwner;
	GLShaderOwner vtsdOwner(GL_VERTEX_SHADER);
	GLShaderOwner frsdOwner(GL_FRAGMENT_SHADER);
	vtsdOwner.compile(VTSD_PATH);
	frsdOwner.compile(FRSD_PATH);
	programOwner = GLProgramOwner(true);
	GLuint programHolder = programOwner.get();
	GLAttachShaderOwner attachOwner(programHolder);
	attachOwner.attach(vtsdOwner.get());
	attachOwner.attach(frsdOwner.get());
	programOwner.link();
	programOwner.unUse();
	iVCoord = glGetAttribLocation(
		programHolder, "iVCoord");
	iVNormal = glGetAttribLocation(
		programHolder, "iVNormal");
	iVTexCoord = glGetAttribLocation(
		programHolder, "iVTexCoord");
	uProjModelView = glGetUniformLocation(
		programHolder, "projViewModelMat");
	uSampler = glGetUniformLocation(
		programHolder, "sampler"
	);
	uLitColor = glGetUniformLocation(
		programHolder, "lit.color"
	);
	uLitDirection = glGetUniformLocation(
		programHolder, "lit.direction"
	);
	uLitIntensity = glGetUniformLocation(
		programHolder, "lit.intensity"
	);
	uCameraPosition = glGetUniformLocation(
		programHolder, "cameraPosition"
	);
}