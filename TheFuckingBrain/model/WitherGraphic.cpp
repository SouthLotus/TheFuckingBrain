#include <gl\glew.h>
#include <gl\freeglut.h>
#include <glm\glm.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <vector>
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cstdio>
#include "WitherGraphic.hpp"
#include "..\tool\SkeletalAnimParser.hpp"
#include "..\tool\MathTool.hpp"
#include "..\tool\SkeletalModel.hpp"

WitherGraphic::WitherGraphic():
	shader(Anim1Shader::create())
{
	initVAO();
	
}

void WitherGraphic::initVAO()
{
	const char *actionName = "Armature|bulletbeat";
	SkeletalModel skmodel("D:/anim.fbx", aiProcess_Triangulate);
	maxAffectBones = skmodel.getMaxAffectBones();
	ticksPerSecond = skmodel.getTicksPerSecond(actionName);
	duration = skmodel.getDuration(actionName);
	numBones = skmodel.getMaxBones(actionName);
	std::vector<glm::vec3> coords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> iks;
	std::vector<glm::vec3> ika;
	std::vector<glm::vec3> ikd;
	std::vector<int> bIDs;
	std::vector<float> bWeights;
	skmodel.getVerticesData(coords, normals, bIDs, bWeights);
	skmodel.getTransformMatrices(actionName, mats);
	skmodel.getMaterial3V(iks, AI_MATKEY_COLOR_SPECULAR);
	skmodel.getMaterial3V(ikd, AI_MATKEY_COLOR_DIFFUSE);
	skmodel.getMaterial3V(ika, AI_MATKEY_COLOR_AMBIENT);
	numVertices = coords.size();
	/*for (int i = 0; i < bIDs.size(); i+=3) {
		printf("%f+%f+%f = %f\n", bWeights[i], bWeights[i+1], bWeights[i+2],
			bWeights[i]+bWeights[i+1]+bWeights[i+2]);
	}*/
	int coordsSize = coords.size() * sizeof(glm::vec3);
	int normalsSize = normals.size() * sizeof(glm::vec3);
	int iksSize = iks.size() * sizeof(glm::vec3);
	int ikdSize = ikd.size() * sizeof(glm::vec3);
	int ikaSize = ika.size() * sizeof(glm::vec3);
	int bWeightsSize = bWeights.size() * sizeof(float);
	int bIDsSize = bIDs.size() * sizeof(int);
	int totalSize = coordsSize + normalsSize 
		+ bIDsSize + bWeightsSize + iksSize + ikdSize + ikaSize;
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
	offset += bWeightsSize;
	glBufferSubData(GL_ARRAY_BUFFER,
		offset, ikaSize, ika.data());
	offset += ikaSize;
	glBufferSubData(GL_ARRAY_BUFFER,
		offset, ikdSize, ikd.data());
	offset += ikdSize;
	glBufferSubData(GL_ARRAY_BUFFER,
		offset, iksSize, iks.data());
	glEnableVertexAttribArray(shader.getICoord());
	glEnableVertexAttribArray(shader.getINormal());
	glEnableVertexAttribArray(shader.getIBIDs(0));
	glEnableVertexAttribArray(shader.getIBIDs(1));
	glEnableVertexAttribArray(shader.getIBWeights(0));
	glEnableVertexAttribArray(shader.getIBWeights(1));
	glEnableVertexAttribArray(shader.getIks());
	glEnableVertexAttribArray(shader.getIka());
	glEnableVertexAttribArray(shader.getIkd());

	glVertexAttribPointer(
		shader.getICoord(), 3, GL_FLOAT, false,
		sizeof(glm::vec3), (void *)0);
	glVertexAttribPointer(
		shader.getINormal(), 3, GL_FLOAT, false,
		sizeof(glm::vec3), (void *)(coordsSize));
	//tinh so phan tu attrb
	int numEB0 = maxAffectBones;
	if (maxAffectBones > 3) {
		numEB0 = 3;
		int numEB1 = maxAffectBones - 3;
		glVertexAttribIPointer(
			shader.getIBIDs(1), numEB1, GL_INT,
			maxAffectBones * sizeof(int), 
			(void *)(coordsSize + normalsSize + sizeof(glm::vec3)));
		glVertexAttribPointer(
			shader.getIBWeights(1), numEB1, GL_FLOAT,
			false, maxAffectBones * sizeof(float), 
			(void *)(coordsSize + normalsSize + bIDsSize + sizeof(glm::vec3)));
	}
	glVertexAttribIPointer(
		shader.getIBIDs(0), numEB0, GL_INT,
		maxAffectBones * sizeof(int), (void *)(coordsSize + normalsSize));
	glVertexAttribPointer(
		shader.getIBWeights(0), numEB0, GL_FLOAT,
		false, maxAffectBones * sizeof(float), (void *)(coordsSize + 
			normalsSize + bIDsSize));
	glVertexAttribPointer(
		shader.getIka(), 3, GL_FLOAT, false,
		sizeof(glm::vec3), (void *)(coordsSize + normalsSize + 
			bIDsSize + bWeightsSize));
	glVertexAttribPointer(
		shader.getIkd(), 3, GL_FLOAT, false,
		sizeof(glm::vec3), (void *)(coordsSize + normalsSize + 
			bIDsSize + bWeightsSize + ikaSize));
	glVertexAttribPointer(
		shader.getIks(), 3, GL_FLOAT, false,
		sizeof(glm::vec3), (void *)(coordsSize + normalsSize +
			bIDsSize + bWeightsSize + ikaSize + ikdSize));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void WitherGraphic::render(
	Camera & camera, DirectionalLight & light)
{
	glm::mat4 worldTrans = glm::translate(glm::mat4(1.f), translate) *
		glm::toMat4(rotate) * glm::scale(glm::mat4(1.f), scale);
	glm::mat4 mat = *camera.getProj() * *camera.getLookAt();
	glBindVertexArray(vaoOwner.get(0));
	shader.useProgram();
	glUniformMatrix4fv(shader.getProjViewModel(),
		1, false, &mat[0][0]);
	glUniformMatrix4fv(shader.getWorldTrans(),
		1, false, &worldTrans[0][0]);
	glUniform3fv(shader.getLitColor(), 1, (GLfloat *)light.getColor());
	glUniform3fv(shader.getLitDirection(), 1,
		(GLfloat *)light.getDirection());
	glUniform1f(shader.getLitIntensity(), light.getIntensity());
	glUniform3fv(shader.getCameraPosition(), 1, (GLfloat *)camera.getPos());
	if (isAnim) {
		double dtime =
			(matht::currentTimeMillis() - beginTime) / 1000.f;
		int ticks = dtime * ticksPerSecond;
		if (ticks < duration+1) {
			glUniformMatrix4fv(shader.getVTrans(),
				numBones, GL_FALSE, (GLfloat *)mats[ticks].data());
		}
		else {
			beginTime = matht::currentTimeMillis();
		}
	}
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	shader.unUseProgram();
	glBindVertexArray(0);
}

void WitherGraphic::beginAnim()
{
	beginTime = matht::currentTimeMillis();
	isAnim = true;
}

void WitherGraphic::endAnim()
{
	beginTime = 0;
	isAnim = false;
}

void WitherGraphic::setTranslate(glm::vec3 vec)
{
	translate = vec;
}

void WitherGraphic::setScale(glm::vec3 vec)
{
	scale = vec;
}

void WitherGraphic::setRotate(glm::tquat<float> quat)
{
	rotate = quat;
}
