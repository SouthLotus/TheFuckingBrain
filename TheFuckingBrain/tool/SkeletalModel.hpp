#ifndef __skeletal_model
#define __skeletal_model
#include "SkeletalAnimParser.hpp"
#include "..\model\SkeletalAnimation.hpp"
#include <vector>
#include <assimp/scene.h>
class SkeletalModel {
private:
	Assimp::Importer importer;
	SkeletalAnimParser parser;
public:
	SkeletalModel(const char *file, int howToWork);
	~SkeletalModel() = default;
	
	void SkeletalModel::getMaterial3V(
		std::vector<glm::vec3>& propertyValueVector,
		const char * pKey, int type, int idx);
	void SkeletalModel::getMaterial4V(
		std::vector<glm::vec4>& propertyValueVector,
		const char * pKey, int type, int idx);

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
	unsigned int getTicksPerSecond(const char *animName);
	unsigned int getDuration(const char *animName);
	unsigned int getMaxAffectBones();
	unsigned int getMaxBones(const char *animName);
	void SkeletalModel::getAnimation(
		const char * name, SkeletalAnimation & anim);
	void SkeletalModel::getAnimations(
		std::map<std::string, SkeletalAnimation>& anims);
};
#endif