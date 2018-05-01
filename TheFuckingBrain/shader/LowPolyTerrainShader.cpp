#include "LowPolyTerrainShader.hpp"
#include <gl\glew.h>
#include <gl\freeglut.h>
#include <iostream>
#include <string>
#include "..\tool\ShaderTool.hpp"
#include "..\owner\GLProgramOwner.hpp"
#include "..\owner\GLShaderCompilerOwner.hpp"
#include "..\owner\GLShaderOwner.hpp"
#include "..\owner\GLAttachShaderOwner.hpp"

const char *LowPolyTerrainShader::VTSD_PATH = "shader/terrain_vtsd.vert";
const char *LowPolyTerrainShader::FRSD_PATH = "shader/terrain_frsd.frag";

void LowPolyTerrainShader::init()
{
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
	uProjModelView = glGetUniformLocation(
		programHolder, "projViewModelMat");
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
	uSmp = glGetUniformLocation(
		programHolder, "smp"
	);
	uMapSize = glGetUniformLocation(
		programHolder, "mapSize"
	);
}

void LowPolyTerrainShader::useProgram()
{
	programOwner.use();
}

void LowPolyTerrainShader::unUseProgram()
{
	programOwner.unUse();
}

int LowPolyTerrainShader::getIVCoord() const
{
	return iVCoord;
}

int LowPolyTerrainShader::getIVNormal() const
{
	return iVNormal;
}

int LowPolyTerrainShader::getUProjModelView() const
{
	return uProjModelView;
}

int LowPolyTerrainShader::getULitColor() const
{
	return uLitColor;
}

int LowPolyTerrainShader::getULitDirection() const
{
	return uLitDirection;
}

int LowPolyTerrainShader::getULitIntensity() const
{
	return uLitIntensity;
}

int LowPolyTerrainShader::getUCameraPosition() const
{
	return uCameraPosition;
}

int LowPolyTerrainShader::getUSmp() const {
	return uSmp;
}

int LowPolyTerrainShader::getUMapSize() const {
	return uMapSize;
}
