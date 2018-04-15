#include <gl\glew.h>
#include <gl\freeglut.h>
#include <string>
#include <vector>
#include <stb\stb_image.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include "SkyBox.hpp"
#include "..\owner\STBImageOwner.hpp"
#include "..\tool\ModelTool.hpp"
#include "..\owner\GLVAOOwner.hpp"
#include "..\owner\GLVBOOwner.hpp"
#include "..\owner\GLTextureOwner.hpp"


SkyBox::SkyBox() {

}

SkyBox::~SkyBox() {
}

void SkyBox::initVao(std::string modelPath) {
	std::vector<float> coords;
	std::vector<float> normals;
	std::vector<float> tcoords;
	ModelTool tool(modelPath);
	tool.loadModelC(coords);
	int coordsSize = coords.size() * sizeof(float);
	this->numVer = coords.size() / 3;
	vaoOwner = GLVAOOwner(1);
	vboOwner = GLVBOOwner(1);
	GLuint vao = vaoOwner.get(0);
	GLuint vbo = vboOwner.get(0);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER, coordsSize,
		coords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(shader.getIVCoord());
	glVertexAttribPointer(
		shader.getIVCoord(), 3, GL_FLOAT,
		GL_FALSE, 3 * sizeof(float), (void *)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SkyBox::initTexture(std::string *texturePath) {
	texOwner = GLTextureOwner(1);
	GLuint tex = texOwner.get(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	for (int i = 0; i < 6; i++) {
		int texWidth;
		int textHeight;
		int channels;
		STBImageOwner owner(stbi_load(texturePath[i].c_str(), &texWidth,
			&textHeight, &channels, STBI_rgb));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, texWidth, textHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, owner.get());
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SkyBox::render(Camera &camera) {
	glm::mat4 view = glm::mat4(glm::mat3(*camera.getLookAt()));
	glm::mat4 finalMatrix = *camera.getProj() * view;
	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CW);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glBindVertexArray(vaoOwner.get(0));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texOwner.get(0));
	this->shader.useProgram();
	glUniform1i(shader.getUSampler(), 0);
	glUniformMatrix4fv(
		shader.getUProjModelView(), 1, GL_FALSE, &finalMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, this->numVer);
	this->shader.unUseProgram();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW);
}

void SkyBox::init(
	std::string modelPath,
	std::string *texturePath
)
{
	this->shader.init();
	this->initVao(modelPath);
	this->initTexture(texturePath);
}