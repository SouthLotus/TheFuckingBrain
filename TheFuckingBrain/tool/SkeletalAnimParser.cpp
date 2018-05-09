#include "SkeletalAnimParser.hpp"
#include <iostream>
#include <cstdio>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <set>
#include <glm\glm.hpp>
#include <vector>
#define DB std::cout << "nam";

SkeletalAnimParser::SkeletalAnimParser(const aiScene * scene):
	scene(scene)
{
	findMaxKeyFrames();
	findLocalMatrices();
	findTransMatrices();
	findFinalTransMatrices();
	findVerticesData();
	findMaxAffectBones();
	
}

SkeletalAnimParser::SkeletalAnimParser(const char * file, int howToWork)
{
	importer.ReadFile(file, howToWork);
	scene = importer.GetScene();
	findMaxKeyFrames();
	findLocalMatrices();
	findTransMatrices();
	findFinalTransMatrices();
	findVerticesData();
	findMaxAffectBones();
	std::cout << maxKeyFrames << std::endl;
	std::cout << scene->mAnimations[0]->mDuration << std::endl;
}

void SkeletalAnimParser::getVerticesData(
	std::vector<glm::vec3> &coords, 
	std::vector<glm::vec3> &normals, 
	std::vector<glm::vec3> &tcoords, 
	std::vector<int> &bIDs, 
	std::vector<float> &bWeights)
{
	aiMesh **meshes = scene->mMeshes;
	int numMeshes = scene->mNumMeshes;
	for (int i = 0; i < numMeshes; i++) {
		aiMesh *mesh = meshes[i];
		std::vector<VertexData> &versData = verticesData[i];
		aiFace *faces = mesh->mFaces;
		int numFaces = mesh->mNumFaces;
		for (int j = 0; j < numFaces; j++) {
			aiFace &face = faces[j];
			unsigned int *vIndinces = face.mIndices;
			int numVInices = face.mNumIndices;
			for (int vidIndex = 0; vidIndex < numVInices; vidIndex++) {
				int vIndex = vIndinces[vidIndex];
				VertexData &verData = versData[vIndex];
				for (int bIndex = 0; bIndex < maxAffectBones; bIndex++) {
					bIDs.push_back(verData.boneIDs[bIndex]);
					bWeights.push_back(verData.weights[bIndex]);
				}
				coords.push_back(vec3ToGLM(verData.coord));
				normals.push_back(vec3ToGLM(verData.normal));
				tcoords.push_back(vec3ToGLM(verData.tcoord));
			}
		}
	}
}

void SkeletalAnimParser::getVerticesData(
	std::vector<glm::vec3> &coords,
	std::vector<glm::vec3> &normals,
	std::vector<int> &bIDs,
	std::vector<float> &bWeights)
{
	aiMesh **meshes = scene->mMeshes;
	int numMeshes = scene->mNumMeshes;
	for (int i = 0; i < numMeshes; i++) {
		aiMesh *mesh = meshes[i];
		std::vector<VertexData> &versData = verticesData[i];
		aiFace *faces = mesh->mFaces;
		int numFaces = mesh->mNumFaces;
		for (int j = 0; j < numFaces; j++) {
			aiFace &face = faces[j];
			unsigned int *vIndinces = face.mIndices;
			int numVInices = face.mNumIndices;
			for (int vidIndex = 0; vidIndex < numVInices; vidIndex++) {
				unsigned int vIndex = vIndinces[vidIndex];
				VertexData &verData = versData[vIndex];
				int numAffectBones = verData.boneIDs.size();
				for (int bIndex = 0; bIndex < numAffectBones; bIndex++) {
					bIDs.push_back(verData.boneIDs[bIndex]);
					bWeights.push_back(verData.weights[bIndex]);
				}
				coords.push_back(vec3ToGLM(verData.coord));
				normals.push_back(vec3ToGLM(verData.normal));
			}
		}
	}
}

void SkeletalAnimParser::getTransformMatrices(
	std::vector<std::vector<glm::mat4>>& mats)
{
	int numKeys = finalTransMats.size();
	int numBones = finalTransMats[0].size();
	mats.resize(numKeys);
	std::cout << numKeys;
	for (int i = 0; i < numKeys; i++) {
		mats[i].resize(numBones);
		for (int j = 0; j < numBones; j++) {
			mats[i][j] = mat4ToGLM(
				finalTransMats[i][j]);
			/*printMatrix(mats[i][j]);
			printf("\n");*/
		}
	}
} 

unsigned int SkeletalAnimParser::getTicksPerSecond()
{
	return scene->mAnimations[0]->mTicksPerSecond;  
}

unsigned int SkeletalAnimParser::getDuration()
{
	return scene->mAnimations[0]->mDuration;
}

unsigned int SkeletalAnimParser::getMaxKeyFrames()
{
	return maxKeyFrames;
}

unsigned int SkeletalAnimParser::getMaxAffectBones()
{
	return maxAffectBones;
}

unsigned int SkeletalAnimParser::getMaxBones()
{
	return scene->mAnimations[0]->mNumChannels - 1;
}


void SkeletalAnimParser::findMaxKeyFrames()
{
	aiAnimation **anims = scene->mAnimations;
	aiAnimation *anim = anims[0];
	aiNodeAnim **channels = anim->mChannels;
	int numChannels = anim->mNumChannels;
	for (int i = 1; i < numChannels; i++) {
		aiNodeAnim *channel = channels[i];
		int numKeys = channel->mNumRotationKeys;
		if (maxKeyFrames < numKeys) {
			maxKeyFrames = numKeys;
		}
	}
}

void SkeletalAnimParser::findLocalMatrices()
{
	aiMesh **meshes = scene->mMeshes;
	int numMeshes = scene->mNumMeshes;
	for (int i = 0; i < numMeshes; i++) {
		aiMesh *mesh = meshes[i];
		aiBone **bones = mesh->mBones;
		int numBones = mesh->mNumBones;
		for (int j = 0; j < numBones; j++) {
			aiBone *bone = bones[j];
			const char *name = bone->mName.C_Str();
			std::cout << name << std::endl;
			if (!localMats.count(name)) {
				localMats[name] = bone->mOffsetMatrix;
				printMatrix(localMats[name]);
				printf("local\n");
			}
		}
	}
}

void printMatrix(aiMatrix4x4 &m) {
	std::printf("%8.4f %8.4f %8.4f %8.4f\n", m.a1, m.a2, m.a3, m.a4);
	std::printf("%8.4f %8.4f %8.4f %8.4f\n", m.b1, m.b2, m.b3, m.b4);
	std::printf("%8.4f %8.4f %8.4f %8.4f\n", m.c1, m.c2, m.c3, m.c4);
	std::printf("%8.4f %8.4f %8.4f %8.4f\n", m.d1, m.d2, m.d3, m.d4);
}

void printMatrix(glm::mat4 &m) {
	std::printf("%8.4f %8.4f %8.4f %8.4f\n", m[0][0], m[1][0], m[2][0], m[3][0]);
	std::printf("%8.4f %8.4f %8.4f %8.4f\n", m[0][1], m[1][1], m[2][1], m[3][1]);
	std::printf("%8.4f %8.4f %8.4f %8.4f\n", m[0][2], m[1][1], m[2][2], m[3][2]);
	std::printf("%8.4f %8.4f %8.4f %8.4f\n", m[0][3], m[1][3], m[2][3], m[3][3]);
}

void SkeletalAnimParser::findTransMatrices()
{
	aiAnimation **anims = scene->mAnimations;
	aiAnimation *anim = anims[0];
	aiNodeAnim **channels = anim->mChannels;
	int numChannels = anim->mNumChannels;
	for (int i = 1; i < numChannels; i++) {
		aiNodeAnim *channel = channels[i];
		int numKeys = channel->mNumRotationKeys;
		const char *name = channel->mNodeName.C_Str();
		transMats[name] = std::vector<aiMatrix4x4>(maxKeyFrames);
		std::cout << name << std::endl;
		for (int j = 0; j < numKeys; j++) {
			aiMatrix4x4 transMat;
			aiMatrix4x4 scalMat;
			aiVector3D &trans = channel->mPositionKeys[j].mValue;
			aiVector3D &scal = channel->mScalingKeys[j].mValue;
			aiQuaternion &quad = channel->mRotationKeys[j].mValue;
			aiMatrix4x4::Translation(trans, transMat);
			aiMatrix4x4::Scaling(scal, scalMat);
			aiMatrix4x4 rotMat(quad.GetMatrix());
			//printf("%f, %f, %f, %f\n", quad.x, quad.y, quad.z, quad.w);
			//printf("%f, %f, %f\n", trans.x, trans.y, trans.z);
			//printf("%f, %f, %f\n", scal.x, scal.y, scal.z);
			transMats[name][j] = scalMat * rotMat * transMat;
			//printMatrix(rotMat);
			std::cout << channel->mRotationKeys[j].mTime << std::endl;
			std::cout << "------------------------" << std::endl;
		}
	}
} 

void SkeletalAnimParser::findFinalTransMatrices()
{
	aiAnimation **anims = scene->mAnimations;
	aiAnimation *anim = anims[0];
	aiNodeAnim **channels = anim->mChannels;
	int numChannels = anim->mNumChannels;
	finalTransMats.resize(maxKeyFrames);
	for (int i = 1; i < numChannels; i++) {
		aiNodeAnim *channel = channels[i];
		const char *name = channel->mNodeName.C_Str();
		aiNode &curNode = *scene->mRootNode->FindNode(name);
		aiMatrix4x4 &localMat = localMats[name];
		int numKeys = channel->mNumRotationKeys;
		int boneIndex = i - 1;
		boneIndices[name] = boneIndex;
		for (int j = 0; j < maxKeyFrames; j++) {
			finalTransMats[j].resize(numChannels - 1);
			finalTransMats[j][boneIndex] = localMat;
			findFinalBoneMatrices(finalTransMats[j][boneIndex],
				curNode, j);
		}
	}
}
/**


*/
void SkeletalAnimParser::findVerticesData()
{
	aiMesh **meshes = scene->mMeshes;
	int numMeshes = scene->mNumMeshes;
	verticesData.resize(numMeshes);
	for (int i = 0; i < numMeshes; i++) {
		aiMesh *mesh = meshes[i];
		aiBone **bones = mesh->mBones;
		std::vector<VertexData> &versData = verticesData[i];
		int numBones = mesh->mNumBones;
		int numVertices = mesh->mNumVertices;
		versData.resize(numVertices);
		for (int vIndex = 0; vIndex < numVertices; vIndex++) {
			VertexData vData;
			vData.coord = mesh->mVertices[vIndex];
			vData.normal = mesh->mNormals[vIndex];
			if(mesh->HasTextureCoords(0))
				vData.tcoord = mesh->mTextureCoords[0][vIndex];
			versData[vIndex] = vData;
		}
		for (int j = 0; j < numBones; j++) { 
			aiBone *bone = bones[j];
			const char *name = bone->mName.C_Str();
			int boneTransIndex = boneIndices[name];
			aiVertexWeight *weights = bone->mWeights;
			int numWeights = bone->mNumWeights;	
			for (int wIndex = 0; wIndex < numWeights; wIndex++) {
				aiVertexWeight &weight = weights[wIndex];
				int vIndex = weight.mVertexId;
				float vWeight = weight.mWeight;
				versData[vIndex].boneIDs.push_back(boneTransIndex);
				versData[vIndex].weights.push_back(vWeight);
			}
		}
	}
	//normalize weight
	for (int i = 0; i < numMeshes; i++) {
		std::vector<VertexData> &versData = verticesData[i];
		int numVersData = versData.size();
		for (int j = 0; j < numVersData; j++) {
			VertexData &verData = versData[j];
			int numAffectBones = verData.boneIDs.size();
			float sum = 0;
			for (int bIndex = 0; bIndex < numAffectBones; bIndex++) {
				sum += verData.weights[bIndex];
			}
			if (sum == 0) {
				continue;
			}
			for (int bIndex = 0; bIndex < numAffectBones; bIndex++) {
				verData.weights[bIndex] /= sum;
			}
		}
	}
}

void SkeletalAnimParser::findMaxAffectBones()
{
	int numMeshes = scene->mNumMeshes;
	for (int i = 0; i < numMeshes; i++) {
		std::vector<VertexData> &versData = verticesData[i];
		int numVersData = versData.size();
		for (int j = 0; j < numVersData; j++) {
			VertexData &verData = versData[j];
			int numAffectBones = verData.boneIDs.size();
			if (numAffectBones > maxAffectBones) {
				maxAffectBones = numAffectBones;
			}
		}
	}
	for (int i = 0; i < numMeshes; i++) {
		std::vector<VertexData> &versData = verticesData[i];
		int numVersData = versData.size();
		for (int j = 0; j < numVersData; j++) {
			VertexData &verData = versData[j];
			int numAffectBones = verData.boneIDs.size();
			verData.boneIDs.resize(maxAffectBones, 0);
			verData.weights.resize(maxAffectBones, 0);	
		}
	}
}

void SkeletalAnimParser::findFinalBoneMatrices(
	aiMatrix4x4 & mat, aiNode & node, int keyIndex)
{
	const char *name = node.mName.C_Str();
	aiNode &parent = *node.mParent;
	if (!transMats.count(parent.mName.C_Str())) {
		aiMatrix4x4 inverseMat = localMats[name];
		inverseMat.Inverse();
		mat = inverseMat * transMats[name][keyIndex] * mat;
		return;
	}
	else {
		std::cout << node.mName.C_Str() <<"-->" << parent.mName.C_Str() << std::endl;
		mat = node.mTransformation * transMats[name][keyIndex] * mat;
		findFinalBoneMatrices(mat, parent, keyIndex);
	}
}

glm::vec3 SkeletalAnimParser::vec3ToGLM(aiVector3D & vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}

glm::mat4 SkeletalAnimParser::mat4ToGLM(aiMatrix4x4 & mat)
{
	return glm::mat4(
		mat.a1, mat.b1, mat.c1, mat.d1,
		mat.a2, mat.b2, mat.c2, mat.d2,
		mat.a3, mat.b3, mat.c3, mat.d3,
		mat.a4, mat.b4, mat.c4, mat.d4
	);
}
