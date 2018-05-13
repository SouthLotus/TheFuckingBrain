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
	/*
	1. Tim cac ma tran chuyen tu he model local sang he bone local tuong
	ung voi cac bone (dung map<string,matrix>)
	2. Tim cac ma tran transform cac vertices trong he bone local tuong ung voi tung bone,
	luu tru duoi dang arr[tick][bone]. (tick la thoi gian xay ra transform tuong ung)
	3. Tim cac ma tran transfom vertices trong model local tuong ung void tung arr[tick][boneIndex],
	cac ma tran nay dung de truyen vao shader.
	4. Tim mot mang cua cau truc sau: [vertex | boneIDs | weights]
	vertex la la thuoc tinh cua mot vertex (normal, coord, tex)
	boneIDs la mang index tro den arr[tick][boneID], weights la mang
	muc anh huong cua bone len vertex tuong ung voi tung boneIDs.
	5. Tim so bone toi da anh huong den mot vertex.
	*/
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
	findLocalMatrices(); 
	findTransMatrices();
	findFinalTransMatrices();
	findVerticesData();
	findMaxAffectBones();	
	//std::cout << getDuration() << std::endl;
	//std::cout << maxAffectBones << std::endl;
	printf("nm\n");
	aiNode *a = scene->mRootNode->FindNode("Bone");
	aiNode *b = a->mParent;
	printMatrix(a->mTransformation);

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
					//printf("bbb%d\n", verData.boneIDs[bIndex]);
					//printf("!!!%f\n", verData.weights[bIndex]);
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
	for (int i = 0; i < numKeys; i++) {
		mats[i].resize(numBones);
		for (int j = 0; j < numBones; j++) {
			mats[i][j] = mat4ToGLM(
				finalTransMats[i][j]);
			//printMatrix(mats[i][j]);
			//printf("\n");
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

unsigned int SkeletalAnimParser::getMaxAffectBones()
{
	return maxAffectBones;
}

unsigned int SkeletalAnimParser::getMaxBones()
{
	return scene->mAnimations[0]->mNumChannels - 1;
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
				printf("local %s\n", name);
				printMatrix(localMats[name]);
				
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

void go(aiNode * fnode)
{
	if (fnode == NULL)
		return;
	aiNode ** nodes = fnode->mChildren;
	std::cout << fnode->mName.C_Str() << std::endl; 
	int numNode = fnode->mNumChildren;
	for (int i = 0; i < numNode; i++) {
		go(nodes[i]);
	}
	
}

void SkeletalAnimParser::findTransMatrices()
{
	std::cout << "Trans" << std::endl << std::endl << std::endl;
	aiAnimation **anims = scene->mAnimations;
	aiAnimation *anim = anims[0];
	aiNodeAnim **channels = anim->mChannels;
	double numTicks = anim->mDuration + 1;
	int numChannels = anim->mNumChannels;
	printf("num channel:%d\n", numChannels);
	for (int i = 1; i < numChannels; i++) {
		aiNodeAnim *channel = channels[i];
		int numKeys = channel->mNumRotationKeys;
		const char *name = channel->mNodeName.C_Str();
		transMats[name] = std::vector<aiMatrix4x4>(numTicks);
		std::cout << "Transform" << name << std::endl;
		//for (int kk = 0; kk < channel->mNumRotationKeys; kk++) {
			//printf("%f\n", channel->mPositionKeys[kk].mTime);
		//}
		/*aiMatrix4x4 transMat;
		aiMatrix4x4 scalMat;
		aiVector3D &trans = channel->mPositionKeys[0].mValue;
		aiVector3D &scal = channel->mScalingKeys[0].mValue;
		aiQuaternion &quad = channel->mRotationKeys[0].mValue;
		aiMatrix4x4::Translation(trans, transMat);
		aiMatrix4x4::Scaling(scal, scalMat);
		aiMatrix4x4 rotMat(quad.GetMatrix());
		aiMatrix4x4 offsetMat = scalMat * rotMat * transMat;
		printf("first\n");
		printMatrix(offsetMat);
		printf("%8.4f %8.4f %8.4f %8.4f\n", quad.x, quad.y, quad.z, quad.w);
		offsetMat.Inverse();*/

		for (int j = 0; j < numTicks; j++) {
			aiMatrix4x4 transMat;
			aiMatrix4x4 scalMat;
			aiVector3D trans;
			aiVector3D scal;
			aiQuaternion quad;
			//find
			findTranslation(trans, channel->mPositionKeys, numKeys, j);
			findScale(scal, channel->mScalingKeys, numKeys, j);
			findRotation(quad, channel->mRotationKeys, numKeys, j);
			aiMatrix4x4::Translation(trans, transMat);
			aiMatrix4x4::Scaling(scal, scalMat);
			aiMatrix4x4 rotMat(quad.GetMatrix());
			printf("rot:%f, %f, %f, %f\n", quad.x, quad.y, quad.z, quad.w);
			printf("tran:%f, %f, %f\n", trans.x, trans.y, trans.z);
			//printf("%f, %f, %f\n", scal.x, scal.y, scal.z);
			transMats[name][j] = transMat * scalMat *  rotMat;
			if (strcmp(name, "Bone.001") == 0) {
				aiVector3D vec(4, 10, -3);
				aiMatrix4x4 inv = localMats["Bone"];
				inv.Inverse();
				aiVector3D v = inv * transMats[name][j] * localMats[name] * vec;
				printf("nnnn %8.4f %8.4f %8.4f\n", v.x, v.y, v.z);
			}
			//printMatrix(transMats[name][j]);
			//printMatrix(rotMat);
		}
	}
} 

void SkeletalAnimParser::findFinalTransMatrices()
{
	aiAnimation **anims = scene->mAnimations;
	aiAnimation *anim = anims[0];
	aiNodeAnim **channels = anim->mChannels;
	double numTicks = anim->mDuration + 1;
	int numChannels = anim->mNumChannels;
	finalTransMats.resize(numTicks);
	for (int i = 1; i < numChannels; i++) {
		aiNodeAnim *channel = channels[i];
		const char *name = channel->mNodeName.C_Str();
		aiNode &curNode = *scene->mRootNode->FindNode(name);
		aiMatrix4x4 &localMat = localMats[name];
		int numKeys = channel->mNumRotationKeys;
		int boneIndex = i - 1;
		boneIndices[name] = boneIndex;
		std::cout << name << std::endl;
		for (int j = 0; j < numTicks; j++) {
			finalTransMats[j].resize(numChannels - 1);
			finalTransMats[j][boneIndex] = localMat;
			findFinalBoneMatrices(finalTransMats[j][boneIndex],
				curNode, j);
			//printMatrix(finalTransMats[j][boneIndex]);
			//printf("\n\n");
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
		/*for (int i = 0; i < verticesData.size(); i++) {
			std::vector<VertexData> &data = verticesData[i];
			int num = verticesData[i].size();
			for (int j = 0; j < num; j++) {
				printf("%8.4f %8.4f %8.4f\n", data[j].coord.x,
					data[j].coord.x, data[j].coord.x);
				int numb = data[j].boneIDs.size();
				for (int k = 0; k < numb; k++) {
					printf("bone: %d\n", data[j].boneIDs[k]);
					printf(">> weight: %f\n", data[j].weights[k]);
				}
				printf("-------------------------------------\n");
			}
		}*/
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
	if (strcmp(parent.mName.C_Str(), "Armature") == 0) {
		aiMatrix4x4 inverseMat = node.mTransformation;
		inverseMat.Inverse();
		aiMatrix4x4 inverseMat2 = localMats[name];
		inverseMat2.Inverse();
		mat = inverseMat2 * inverseMat * transMats[name][keyIndex] * mat;
		return;
	}
	else {
		mat = transMats[name][keyIndex] * mat;
		findFinalBoneMatrices(mat, parent, keyIndex);
		printf("%s <-- %s\n", parent.mName.C_Str(), name);
		printMatrix(node.mTransformation);
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

void SkeletalAnimParser::translateInterpolate(
	aiVector3D &v0, aiVector3D &v1, aiVector3D &out, float factor) {
		out = (1-factor) * v0 + factor * v1;
}
void SkeletalAnimParser::scaleInterpolate(
	aiVector3D &v0, aiVector3D &v1, aiVector3D &out, float factor) {
		out = (1-factor) * v0 + factor * v1;
}
void SkeletalAnimParser::rotationInterpolate(
	aiQuaternion &q0, aiQuaternion &q1, aiQuaternion &out, float factor) {
		aiQuaternion::Interpolate(out, q0, q1, factor);
}

void SkeletalAnimParser::findTranslation(
	aiVector3D & vec, aiVectorKey *keys, int numKeys, double time)
{
	double factor;
	aiVectorKey *key0;
	aiVectorKey *key1;
	for (int i = 1; i < numKeys; i++) {
		key1 = &keys[i];
		if (time <= key1->mTime) {
			key0 = &keys[i - 1];
			factor = 
				1.f * (time - key0->mTime) / (key1->mTime - key0->mTime);
			break;
		}
	}
	translateInterpolate(key0->mValue, key1->mValue, vec, factor);
}

void SkeletalAnimParser::findScale(
	aiVector3D & vec, aiVectorKey *keys, int numKeys, double time)
{
	double factor;
	aiVectorKey *key0;
	aiVectorKey *key1;
	for (int i = 1; i < numKeys; i++) {
		key1 = &keys[i];
		if (time <= key1->mTime) {
			key0 = &keys[i - 1];
			factor =
				1.f * (time - key0->mTime) / (key1->mTime - key0->mTime);
			break;
		}
	}
	translateInterpolate(key0->mValue, key1->mValue, vec, factor);
}

void SkeletalAnimParser::findRotation(
	aiQuaternion & quad, aiQuatKey *keys, int numKeys, double time)
{
	double factor;
	aiQuatKey *key0;
	aiQuatKey *key1;
	for (int i = 1; i < numKeys; i++) {
		key1 = &keys[i];
		if (time <= key1->mTime) {
			key0 = &keys[i - 1];
			factor =
				(time - key0->mTime) / (key1->mTime - key0->mTime);
			break;
		}
	}
	//std::cout << factor << "|\n" << std::endl;
	rotationInterpolate(key0->mValue, key1->mValue, quad, factor);
}
