#include "LowPolyShader.hpp"
#include <gl\glew.h>
#include <gl\freeglut.h>
#include "..\tool\ShaderTool.hpp"
#include <iostream>
#include <exception>
#include <string>
#include "..\owner\GLProgramOwner.hpp"
#include "..\owner\GLShaderCompilerOwner.hpp"
#include "..\owner\GLShaderOwner.hpp"
#include "..\owner\GLAttachShaderOwner.hpp"

const char *LowPolyShader::VTSD_PATH = "shader/lowpoly_vtsd.vert";
const char *LowPolyShader::FRSD_PATH = "shader/lowpoly_frsd.frag";

void LowPolyShader::init()
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
	ika = glGetAttribLocation(
		programHolder, "ika"
	);
	ikd = glGetAttribLocation(
		programHolder, "ikd"
	);
	iks = glGetAttribLocation(
		programHolder, "iks"
	);
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

void LowPolyShader::useProgram()
{
	programOwner.use();
}

void LowPolyShader::unUseProgram()
{
	programOwner.unUse();
}

int LowPolyShader::getIVCoord() const
{
	return iVCoord;
}

int LowPolyShader::getIVNormal() const
{
	return iVNormal;
}

int LowPolyShader::getIka() const
{
	return ika;
}

int LowPolyShader::getIkd() const
{
	return ikd;
}

int LowPolyShader::getIks() const
{
	return iks;
}

int LowPolyShader::getUProjModelView() const
{
	return uProjModelView;
}

int LowPolyShader::getULitColor() const
{
	return uLitColor;
}

int LowPolyShader::getULitDirection() const
{
	return uLitDirection;
}

int LowPolyShader::getULitIntensity() const
{
	return uLitIntensity;
}

int LowPolyShader::getUCameraPosition() const
{
	return uCameraPosition;
}
