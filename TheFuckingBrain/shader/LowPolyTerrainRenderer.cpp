#include "LowPolyTerrainRenderer.hpp"
#include <gl\glew.h>
#include <gl\freeglut.h>
#include <string>
#include <vector>
#include <stb\stb_image.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <assimp\material.h>
#include "..\tool\DiamondSquareTerrain.hpp"
#include "..\owner\STBImageOwner.hpp"
#include "..\tool\ModelTool.hpp"
#include "..\owner\GLVAOOwner.hpp"
#include "..\owner\GLVBOOwner.hpp"
#include "..\owner\GLTextureOwner.hpp"
#include "..\object\DirectionalLight.hpp"

void LowPolyTerrainRenderer::initVao() {
	std::vector<glm::vec3> coords;
	std::vector<glm::vec3> normals;
	std::vector<std::vector<float>> heights;
	DiamondSquareTerrain terrain(9, 255);
	terrain.flatInValue(100, 100, 150);
	terrain.flatInValue(150, 150, 200);
	terrain.doGaussainBlur(3, 20);
	terrain.saveBMP("D:/nam.bmp");
	terrain.toResolution(heights, 9);
	terrainMap = LowPolyTerrainMap(heights, 1000);
	//terrainMap = LowPolyTerrainMap("D:/nam.terr");
	terrainMap.saveToFile("D:/nam.terr");
	terrainMap.toTriangleMesh(coords, normals);
	int coordsSize = coords.size() * sizeof(glm::vec3);
	int normalsSize = normals.size() * sizeof(glm::vec3);
	int bufferSize = coordsSize + normalsSize;
	this->numVer = coords.size();
	vaoOwner = GLVAOOwner(1);
	vboOwner = GLVBOOwner(1);
	GLuint vao = vaoOwner.get(0);
	GLuint vbo = vboOwner.get(0);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER, bufferSize,
		NULL, GL_STATIC_DRAW);
	glBufferSubData(
		GL_ARRAY_BUFFER, 0,
		coordsSize, coords.data());
	glBufferSubData(
		GL_ARRAY_BUFFER, coordsSize,
		normalsSize, normals.data());
	glEnableVertexAttribArray(shader.getIVCoord());
	glEnableVertexAttribArray(shader.getIVNormal());
	glVertexAttribPointer(
		shader.getIVCoord(), 3, GL_FLOAT,
		GL_FALSE, sizeof(glm::vec3), (void *)0);
	glVertexAttribPointer(
		shader.getIVNormal(), 3, GL_FLOAT,
		GL_FALSE, sizeof(glm::vec3), (void *)coordsSize);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LowPolyTerrainRenderer::initTexture()
{
	STBImageOwner img("D:/cam.bmp", 3);
	texOwner = GLTextureOwner(1);
	GLuint tex = texOwner.get(0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, img.getW(), img.getH(), 0,
		GL_RGB, GL_UNSIGNED_BYTE, img.get());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void LowPolyTerrainRenderer::render(Camera &camera, DirectionalLight &lit) {
	glm::mat4 finalMatrix = *camera.getProj() * *camera.getLookAt();
	glBindVertexArray(vaoOwner.get(0));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texOwner.get(0));
	this->shader.useProgram();
	glUniform3fv(shader.getULitColor(), 1, (GLfloat *)lit.getColor());
	glUniform3fv(shader.getULitDirection(), 1, (GLfloat *)lit.getDirection());
	glUniform3fv(shader.getUCameraPosition(), 1, (GLfloat *)camera.getPos());
	glUniform1f(shader.getULitIntensity(), lit.getIntensity());
	glUniformMatrix4fv(
		shader.getUProjModelView(), 1, GL_FALSE, &finalMatrix[0][0]);
	glUniform1i(shader.getUSmp(), 0);
	glUniform1f(shader.getUMapSize(), terrainMap.getSize());
	glDrawArrays(GL_TRIANGLES, 0, this->numVer);
	this->shader.unUseProgram();
	glBindVertexArray(0);
}

void LowPolyTerrainRenderer::init()
{
	this->shader.init();
	this->initVao();
	this->initTexture();
}

float LowPolyTerrainRenderer::getHeight(float x, float z)
{
	return terrainMap.getHeight(x, z);
}