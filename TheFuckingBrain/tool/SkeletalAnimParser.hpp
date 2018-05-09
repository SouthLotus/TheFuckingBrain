#ifndef __skeletal_anim_parser
#define __skeletal_anim_parser
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <vector>
#include <map>
#include <set>
#include <glm/glm.hpp>
class SkeletalAnimParser {
private:
	struct VertexData
	{
		aiVector3D coord;
		aiVector3D normal;
		aiVector3D tcoord;
		std::vector<int> boneIDs;
		std::vector<float> weights;
	};
	const aiScene *scene;
	Assimp::Importer importer;
	unsigned int maxKeyFrames = 0;
	unsigned int maxAffectBones = 0;
	std::map<std::string, int> boneIndices;
	std::map<std::string, aiMatrix4x4> localMats;
	std::map<std::string, std::vector<aiMatrix4x4>> transMats;

	/*
	key1 : matb1 matb2 matb3 ...
	key2 : matb1' matb2' matb3'
	.................
	*/
	std::vector<std::vector<aiMatrix4x4>> finalTransMats;
	std::vector<std::vector<VertexData>> verticesData;
	void findMaxKeyFrames();
	void findLocalMatrices();
	void findTransMatrices();
	void findFinalTransMatrices();
	void findVerticesData();
	void findMaxAffectBones();
	void findFinalBoneMatrices(
		aiMatrix4x4 &mat, aiNode &node, int keyIndex);
	glm::vec3 vec3ToGLM(aiVector3D &vec);
	glm::mat4 mat4ToGLM(aiMatrix4x4 &mat);

public:
	SkeletalAnimParser() = default;
	~SkeletalAnimParser() = default;
	SkeletalAnimParser(const aiScene * scene);
	SkeletalAnimParser(const char *file, int howToWork);
	void getVerticesData(
		std::vector<glm::vec3> &coods, std::vector<glm::vec3> &normals,
		std::vector<glm::vec3> &tcoords, std::vector<int> &bIDs,
		std::vector<float> &weights);
	void getVerticesData(
		std::vector<glm::vec3> &coods, std::vector<glm::vec3> &normals,
		std::vector<int> &bIDs,
		std::vector<float> &weights);
	void getTransformMatrices(
		std::vector<std::vector<glm::mat4>> &mats);
	unsigned int getTicksPerSecond();
	unsigned int getDuration();
	unsigned int getMaxKeyFrames();
	unsigned int getMaxAffectBones();
	unsigned int getMaxBones();
}; 
void printMatrix(aiMatrix4x4 &m);
void printMatrix(glm::mat4 &m);
#endif
