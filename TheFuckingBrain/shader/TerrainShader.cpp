#include "TerrainShader.hpp"
#include <gl\glew.h>
#include <gl\freeglut.h>
#include <iostream>
#include <string>
#include "..\tool\ShaderTool.hpp"
#include "..\owner\GLProgramOwner.hpp"
#include "..\owner\GLShaderCompilerOwner.hpp"
#include "..\owner\GLShaderOwner.hpp"
#include "..\owner\GLAttachShaderOwner.hpp"

const char *TerrainShader::VTSD_PATH = "shader/terrain_vtsd.vert";
const char *TerrainShader::FRSD_PATH = "shader/terrain_frsd.frag";

void TerrainShader::init()
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
	/*std::cout << iVCoord << std::endl;
	std::cout << iVNormal << std::endl;
	std::cout << ika << std::endl;
	std::cout << ikd << std::endl;
	std::cout << iks << std::endl;
	std::cout << uProjModelView << std::endl;
	std::cout << uLitColor << std::endl;
	std::cout << uLitDirection << std::endl;
	std::cout << uLitIntensity << std::endl;
	std::cout << uCameraPosition << std::endl;*/
}

void TerrainShader::useProgram()
{
	programOwner.use();
}

void TerrainShader::unUseProgram()
{
	programOwner.unUse();
}

int TerrainShader::getIVCoord() const
{
	return iVCoord;
}

int TerrainShader::getIVNormal() const
{
	return iVNormal;
}

int TerrainShader::getUProjModelView() const
{
	return uProjModelView;
}

int TerrainShader::getULitColor() const
{
	return uLitColor;
}

int TerrainShader::getULitDirection() const
{
	return uLitDirection;
}

int TerrainShader::getULitIntensity() const
{
	return uLitIntensity;
}

int TerrainShader::getUCameraPosition() const
{
	return uCameraPosition;
}
