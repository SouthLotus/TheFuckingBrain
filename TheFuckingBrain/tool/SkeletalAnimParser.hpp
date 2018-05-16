#ifndef __skeletal_anim_parser
#define __skeletal_anim_parser
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <vector>
#include <map>
#include <set>
#include <glm/glm.hpp>
#include "..\model\SkeletalAnimation.hpp"
class SkeletalAnimParser {
private:

	/*
	boneIDs mang cac bone ma co anh huong den vertex
	weights muc do anh huong tuong ung voi cac bone
	vertex' = (M1 * w1 + M2 * w2 + M3 * w3 + ... + Mn * wn) * vertex
	M1, w1 lan luot la ma tran chuyen doi bone do va muc do anh huong
	*/
	static struct VertexData
	{
		aiVector3D coord;
		aiVector3D normal;
		aiVector3D tcoord;
		std::vector<int> boneIDs;
		std::vector<float> weights;
	};

	static struct AnimationData {
		std::string name;
		std::map<std::string, std::vector<aiMatrix4x4>> transMats;
		std::vector<std::vector<aiMatrix4x4>> finalTransMats;
		double duration;
		double ticksPerSecond;
		int numChannels;
		int index;
	};
	const aiScene *scene;
	unsigned int maxAffectBones = 0;
	//Ten bone - boneID trong finalTransMats
	std::map<std::string, int> boneIndices;
	//ten bone - matrix chuyen he model local sang he bone local
	std::map<std::string, aiMatrix4x4> localMats;
	//ten bone - matrix transform bone nay trong he local cua bone cha.
	//std::map<std::string, std::vector<aiMatrix4x4>> transMats;
	/*
	2D Array
	boneID bat dau tu 0 (index trong mang)
	       b0ID   b1ID   b2ID   ...
	key1 : matb0  matb1  matb2 ...
	key2 : matb0' matb1' matb2'...
	key3 : .......................
	..............................
	key size = aiAnimation.mDuration + 1
	num bone = aiNodeAnim.mChannels - 1 (vi co channel 0 ten la Armature)
	*/
	//std::vector<std::vector<aiMatrix4x4>> finalTransMats;
	/*
	2D Array
	Mesh0 : VD0  VD1  VD2  VD3  VD4
	Mesh1 : VD0' VD1' VD3
	Mesh2 : VD0''VD1''VD2''VD3''
	.................................
	mang VDi trong moi Mesh tuong ung voi aiMesh.mVertices.
	mang Meshi tuong ung voi aiScene.mMeshes
	*/
	std::map<std::string, AnimationData> animData;
	std::vector<std::vector<VertexData>> verticesData;
	void translateInterpolate(
		aiVector3D &v0, aiVector3D &v1, aiVector3D &out, float factor);
	void scaleInterpolate(
		aiVector3D &v0, aiVector3D &v1, aiVector3D &out, float factor);
	void rotationInterpolate(
		aiQuaternion &q0, aiQuaternion &q1, aiQuaternion &out, float factor);
	void findTranslation(
		aiVector3D & vec, aiVectorKey *keys, int numKeys, double time);
	void findScale(
		aiVector3D & vec, aiVectorKey *keys, int numKeys, double time);
	void findRotation(
		aiQuaternion & quad, aiQuatKey *keys, int numKeys, double time);
	void findLocalMatrices();
	void findTransMatrices(AnimationData &ad);
	void findFinalTransMatrices(AnimationData &ad);
	void findVerticesData();
	void findMaxAffectBones();
	void findFinalBoneMatrices(
		aiMatrix4x4 &mat, aiNode &node, int keyIndex,
		std::map<std::string, std::vector<aiMatrix4x4>> &transMats);
	void findAnimationData();
	glm::vec3 vec3ToGLM(aiVector3D &vec);
	glm::mat4 mat4ToGLM(aiMatrix4x4 &mat);

public:
	SkeletalAnimParser() = default;
	~SkeletalAnimParser() = default;
	SkeletalAnimParser(const aiScene * scene);
	void getVerticesData(
		std::vector<glm::vec3> &coods, std::vector<glm::vec3> &normals,
		std::vector<glm::vec3> &tcoords, std::vector<int> &bIDs,
		std::vector<float> &weights);
	void getVerticesData(
		std::vector<glm::vec3> &coods, std::vector<glm::vec3> &normals,
		std::vector<int> &bIDs,
		std::vector<float> &weights);
	void getTransformMatrices(
		const char *animName,
		std::vector<std::vector<glm::mat4>> &mats);
	void getAnimation(const char *name, SkeletalAnimation &anim);
	void getAnimations(std::map<std::string,SkeletalAnimation> &anims);
	unsigned int getTicksPerSecond(const char *animName);
	unsigned int getDuration(const char *animName);
	unsigned int getMaxAffectBones();
	unsigned int getMaxBones(const char *animName);
}; 
void printMatrix(aiMatrix4x4 &m);
void printMatrix(glm::mat4 &m);
#endif
