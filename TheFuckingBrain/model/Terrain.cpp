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
#include "Terrain.hpp"
#include "..\owner\STBImageOwner.hpp"
#include "..\tool\ModelTool.hpp"
#include "..\owner\GLVAOOwner.hpp"
#include "..\owner\GLVBOOwner.hpp"
#include "..\owner\GLTextureOwner.hpp"
#include "..\object\DirectionalLight.hpp"

void Terrain::initVao() {
	std::vector<glm::vec3> coords;
	std::vector<glm::vec3> normals;
	DiamondSquareTerrain terrain(8, 150);
	terrain.toTriangleMesh(1000, 8, coords, normals);
	int coordsSize =coords.size() * sizeof(glm::vec3);
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

void Terrain::render(Camera &camera, DirectionalLight &lit) {
	glm::mat4 finalMatrix = *camera.getProj() * *camera.getLookAt();
	glBindVertexArray(vaoOwner.get(0));
	this->shader.useProgram();
	glUniform3fv(shader.getULitColor(), 1, (GLfloat *)lit.getColor());
	glUniform3fv(shader.getULitDirection(), 1, (GLfloat *)lit.getDirection());
	glUniform3fv(shader.getUCameraPosition(), 1, (GLfloat *)camera.getPos());
	glUniform1f(shader.getULitIntensity(), lit.getIntensity());
	glUniformMatrix4fv(
		shader.getUProjModelView(), 1, GL_FALSE, &finalMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, this->numVer);
	this->shader.unUseProgram();
	glBindVertexArray(0);
}

void Terrain::init()
{
	this->shader.init();
	this->initVao();
}
