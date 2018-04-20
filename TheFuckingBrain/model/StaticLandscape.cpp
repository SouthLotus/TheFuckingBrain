#include <gl\glew.h>
#include <gl\freeglut.h>
#include <string>
#include <vector>
#include <stb\stb_image.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <assimp\material.h>
#include "StaticLandscape.hpp"
#include "..\owner\STBImageOwner.hpp"
#include "..\tool\ModelTool.hpp"
#include "..\owner\GLVAOOwner.hpp"
#include "..\owner\GLVBOOwner.hpp"
#include "..\owner\GLTextureOwner.hpp"
#include "..\object\DirectionalLight.hpp"

void StaticLandscape::initVao(std::string modelPath) {
	std::vector<float> coords;
	std::vector<float> normals;
	std::vector<float> kas;
	std::vector<float> kds;
	std::vector<float> kss;
	ModelTool tool(modelPath);
	tool.loadModelCN(coords, normals);
	tool.loadModelM3V(kas, AI_MATKEY_COLOR_AMBIENT);
	tool.loadModelM3V(kds, AI_MATKEY_COLOR_DIFFUSE);
	tool.loadModelM3V(kss, AI_MATKEY_COLOR_SPECULAR);
	int coordsSize =coords.size() * sizeof(float);
	int normalsSize = normals.size() * sizeof(float);
	int kassSize = kas.size() * sizeof(float);
	int kdssSize = kds.size() * sizeof(float);
	int kssSize = kss.size() * sizeof(float);
	int bufferSize = coordsSize + normalsSize + kassSize +
		kdssSize + kssSize;
	this->numVer = coords.size() / 3;
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
	glBufferSubData(
		GL_ARRAY_BUFFER, coordsSize + normalsSize,
		 kassSize, kas.data());
	glBufferSubData(
		GL_ARRAY_BUFFER, coordsSize + normalsSize + kassSize,
		kdssSize, kds.data());
	glBufferSubData(
		GL_ARRAY_BUFFER, coordsSize + normalsSize + kassSize + kdssSize,
		kssSize, kss.data());
	glEnableVertexAttribArray(shader.getIVCoord());
	glEnableVertexAttribArray(shader.getIVNormal());
	glEnableVertexAttribArray(shader.getIka());
	glEnableVertexAttribArray(shader.getIkd());
	glEnableVertexAttribArray(shader.getIks());
	glVertexAttribPointer(
		shader.getIVCoord(), 3, GL_FLOAT, 
		GL_FALSE, 3 * sizeof(float), (void *)0);
	glVertexAttribPointer(
		shader.getIVNormal(), 3, GL_FLOAT, 
		GL_FALSE, 3 * sizeof(float), (void *)coordsSize);
	glVertexAttribPointer(
		shader.getIka(), 3, GL_FLOAT,
		GL_FALSE, 3 * sizeof(float), (void *)(coordsSize + normalsSize));
	glVertexAttribPointer(
		shader.getIkd(), 3, GL_FLOAT,
		GL_FALSE, 3 * sizeof(float), (void *)(coordsSize + normalsSize + kassSize));
	glVertexAttribPointer(
		shader.getIks(), 3, GL_FLOAT,
		GL_FALSE, 3 * sizeof(float), (void *)(coordsSize + normalsSize + kassSize + kdssSize));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void StaticLandscape::render(Camera &camera, DirectionalLight &lit) {
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

void StaticLandscape::init(
	std::string modelPath
)
{
	this->shader.init();
	this->initVao(modelPath);
}
