#include <gl\glew.h>
#include <gl\freeglut.h>
#include <glm\glm.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <vector>
#include <glm\glm.hpp>
#include <cstdio>
#include "Anim1Renderer.hpp"
#include "..\tool\SkeletalAnimParser.hpp"
#include "..\tool\MathTool.hpp"

Anim1Renderer::Anim1Renderer():
	shader(Anim1Shader::create())
{
	initVAO();
}

void Anim1Renderer::initVAO()
{
	SkeletalAnimParser parser("D:/anim.fbx", aiProcess_Triangulate);
	maxAffectBones = parser.getMaxAffectBones();
	numKeyFrames = parser.getMaxKeyFrames();
	ticksPerSecond = parser.getTicksPerSecond();
	duration = parser.getDuration();
	numBones = parser.getMaxBones();
	std::vector<glm::vec3> coords;
	std::vector<glm::vec3> normals;
	std::vector<int> bIDs;
	std::vector<float> bWeights;
	parser.getVerticesData(coords, normals, bIDs, bWeights);
	parser.getTransformMatrices(mats);
	numVertices = coords.size();
	int coordsSize = coords.size() * sizeof(glm::vec3);
	int normalsSize = normals.size() * sizeof(glm::vec3);
	int bWeightsSize = bWeights.size() * sizeof(float);
	int bIDsSize = bIDs.size() * sizeof(int);
	int totalSize = coordsSize + normalsSize 
		+ bIDsSize + bWeightsSize;
	int offset = 0;
	vaoOwner = GLVAOOwner(1);
	vboOwener = GLVBOOwner(1);
	GLuint vao = vaoOwner.get(0);
	GLuint vbo = vboOwener.get(0);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 
		offset, coordsSize, coords.data());
	offset += coordsSize;
	glBufferSubData(GL_ARRAY_BUFFER, 
		offset, normalsSize, normals.data());
	offset += normalsSize;
	glBufferSubData(GL_ARRAY_BUFFER, 
		offset, bIDsSize, bIDs.data());
	offset += bIDsSize;
	glBufferSubData(GL_ARRAY_BUFFER, 
		offset, bWeightsSize, bWeights.data());
	offset = 0;
	glEnableVertexAttribArray(shader.getICoord());
	glEnableVertexAttribArray(shader.getINormal());
	glEnableVertexAttribArray(shader.getIBIDs());
	glEnableVertexAttribArray(shader.getIBWeights());
	glVertexAttribPointer(
		shader.getICoord(), 3, GL_FLOAT, false, sizeof(glm::vec3), (void *)offset);
	glVertexAttribPointer(
		shader.getINormal(), 3, GL_FLOAT, false, sizeof(glm::vec3), (void *)(offset+=coordsSize));
	glVertexAttribIPointer(
		shader.getIBIDs(), maxAffectBones, GL_INT, maxAffectBones * sizeof(int), (void *)(offset += normalsSize));
	glVertexAttribPointer(
		shader.getIBWeights(), maxAffectBones, GL_FLOAT, false, maxAffectBones * sizeof(float), (void *)(offset += bIDsSize));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Anim1Renderer::render(
	Camera & Camera, DirectionalLight & light)
{
	glm::mat4 mat = *Camera.getProj() * *Camera.getLookAt();
	glBindVertexArray(vaoOwner.get(0));
	shader.useProgram();
	glUniformMatrix4fv(shader.getProjViewModel(),
		1, false, &mat[0][0]);
	if (isAnim) {
		double dtime =
			(matht::currentTimeMillis() - beginTime) / 1000.f;
		int ticks = dtime * ticksPerSecond;
		if (ticks < numKeyFrames) {
			glUniformMatrix4fv(shader.getVTrans(),
				numBones, false, (GLfloat *)mats[ticks].data());
		}
		else {
			beginTime = matht::currentTimeMillis();
		}
	}
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	shader.unUseProgram();
	glBindVertexArray(0);
}

void Anim1Renderer::beginAnim()
{
	beginTime = matht::currentTimeMillis();
	isAnim = true;
}

void Anim1Renderer::endAnim()
{
	beginTime = 0;
	isAnim = false;
}
