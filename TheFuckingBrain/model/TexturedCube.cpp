#include <gl\glew.h>
#include <gl\freeglut.h>
#include <string>
#include <vector>
#include <stb\stb_image.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include "TexturedCube.hpp"
#include "..\owner\STBImageOwner.hpp"
#include "..\tool\ModelTool.hpp"
#include "..\owner\GLVAOOwner.hpp"
#include "..\owner\GLVBOOwner.hpp"
#include "..\owner\GLTextureOwner.hpp"
#include "..\object\DirectionalLight.hpp"


TexturedCube::TexturedCube() {
	
}

TexturedCube::~TexturedCube() {
}

void TexturedCube::initVao(std::string modelPath) {
	std::vector<float> coords;
	std::vector<float> normals;
	std::vector<float> tcoords;
	ModelTool tool(modelPath);
	tool.loadModelCNT2D(coords, normals, tcoords);
	int coordsSize =coords.size() * sizeof(float);
	int normalsSize = normals.size() * sizeof(float);
	int tcoordsSize = tcoords.size() *sizeof(float);
	int bufferSize = coordsSize + normalsSize + tcoordsSize;
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
		 tcoordsSize, tcoords.data());
	glEnableVertexAttribArray(shader.getIVCoord());
	glEnableVertexAttribArray(shader.getIVNormal());
	glEnableVertexAttribArray(shader.getIVTexCoord());
	glVertexAttribPointer(
		shader.getIVCoord(), 3, GL_FLOAT, 
		GL_FALSE, 3 * sizeof(float), (void *)0);
	glVertexAttribPointer(
		shader.getIVNormal(), 3, GL_FLOAT, 
		GL_FALSE, 3 * sizeof(float), (void *)coordsSize);
	glVertexAttribPointer(
		shader.getIVTexCoord(), 2, GL_FLOAT,
		GL_FALSE, 2 * sizeof(float), 
		(void *)(normalsSize + coordsSize));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TexturedCube::initTexture(std::string texturePath) {
	int texWidth;
	int textHeight;
	int channels;
	stbi_uc *textureData = stbi_load(
		texturePath.c_str(), &texWidth, &textHeight, &channels, STBI_rgb);
	STBImageOwner cleaner(textureData);
	texOwner = GLTextureOwner(1);
	GLuint tex = texOwner.get(0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, texWidth, 
		textHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TexturedCube::render(Camera &camera, DirectionalLight &lit) {
	glm::mat4 finalMatrix = *camera.getProj() * *camera.getLookAt();
	glBindVertexArray(vaoOwner.get(0));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texOwner.get(0));
	this->shader.useProgram();
	glUniform1i(shader.getUSampler(), 0);
	glUniform3fv(shader.getULitColor(), 1, (GLfloat *)lit.getColor());
	glUniform3fv(shader.getULitDirection(), 1, (GLfloat *)lit.getDirection());
	glUniform3fv(shader.getUCameraPosition(), 1, (GLfloat *)camera.getPos());
	glUniform1f(shader.getULitIntensity(), lit.getIntensity());
	glUniformMatrix4fv(
		shader.getUProjModelView(), 1, GL_FALSE, &finalMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, this->numVer);
	this->shader.unUseProgram();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void TexturedCube::init(
	std::string modelPath,
	std::string texturePath
)
{
	this->shader.init();
	this->initVao(modelPath);
	this->initTexture(texturePath);
}
