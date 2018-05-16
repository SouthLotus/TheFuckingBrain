#include "SkeletalModel.hpp"
#include "..\model\SkeletalAnimation.hpp"
#include <assimp/scene.h>
#include <map>
#include <stdexcept>
#include <stdexcept>
#include <glm/glm.hpp>

SkeletalModel::SkeletalModel(const char * file, int howToWork)
{
	const aiScene *scene = importer.ReadFile(file, howToWork);
	if (scene == nullptr) {
		throw std::runtime_error("SkeletalModel Constructor exception.");
	}
	parser = SkeletalAnimParser(scene);
}

/*Lay material properties cua model co dang AI_MATKEY_COLOR_XXX aiVector3D*/
void SkeletalModel::getMaterial3V(
	std::vector<glm::vec3>& propertyValueVector,
	const char * pKey, int type, int idx)
{
	const aiScene *scene = this->importer.GetScene();
	aiMesh **meshes = scene->mMeshes;
	aiMaterial **materials = scene->mMaterials;
	int numMeshes = scene->mNumMeshes;
	/*Luu tru cac coord, normal cua vertex trong vector<glm::vec3>*/
	for (int meshIndex = 0; meshIndex < numMeshes; meshIndex++) {
		aiMesh *mesh = meshes[meshIndex];
		aiFace *faces = mesh->mFaces;
		aiMaterial *material = materials[mesh->mMaterialIndex];
		aiColor3D color;
		int numFaces = mesh->mNumFaces;
		material->Get(pKey, type, idx, color);
		for (int faceIndex = 0; faceIndex < numFaces; faceIndex++) {
			aiFace face = faces[faceIndex];
			int numVertexIndices = face.mNumIndices;
			for (int vertexIndexInFace = 0; vertexIndexInFace < numVertexIndices; vertexIndexInFace++) {
				propertyValueVector.emplace_back(
					color.r, color.g, color.b
				);
			}
		}
	}
}

/*Lay material properties cua model co dang AI_MATKEY_COLOR_XXX aiVector4D*/
void SkeletalModel::getMaterial4V(
	std::vector<glm::vec4>& propertyValueVector,
	const char * pKey, int type, int idx)
{
	const aiScene *scene = this->importer.GetScene();
	aiMesh **meshes = scene->mMeshes;
	aiMaterial **materials = scene->mMaterials;
	int numMeshes = scene->mNumMeshes;
	/*Luu tru cac coord, normal cua vertex trong vector<glm::vec4>*/
	for (int meshIndex = 0; meshIndex < numMeshes; meshIndex++) {
		aiMesh *mesh = meshes[meshIndex];
		aiFace *faces = mesh->mFaces;
		aiMaterial *material = materials[mesh->mMaterialIndex];
		aiColor4D color;
		int numFaces = mesh->mNumFaces;
		material->Get(pKey, type, idx, color);
		for (int faceIndex = 0; faceIndex < numFaces; faceIndex++) {
			aiFace face = faces[faceIndex];
			int numVertexIndices = face.mNumIndices;
			for (int vertexIndexInFace = 0; vertexIndexInFace < numVertexIndices; vertexIndexInFace++) {
				propertyValueVector.emplace_back(
					color.r, color.g, color.b, color.a);
			}
		}
	}
}

void SkeletalModel::getVerticesData(
	std::vector<glm::vec3>& coods,
	std::vector<glm::vec3>& normals,
	std::vector<glm::vec3>& tcoords,
	std::vector<int>& bIDs, std::vector<float>& weights)
{
	parser.getVerticesData(coods, normals, tcoords, bIDs, weights);
}

void SkeletalModel::getVerticesData(
	std::vector<glm::vec3>& coods, 
	std::vector<glm::vec3>& normals,
	std::vector<int>& bIDs, std::vector<float>& weights)
{
	parser.getVerticesData(coods, normals, bIDs, weights);
}

void SkeletalModel::getTransformMatrices(
	const char * animName, 
	std::vector<std::vector<glm::mat4>>& mats)
{
	parser.getTransformMatrices(animName, mats);
}

unsigned int SkeletalModel::getTicksPerSecond(const char *animName)
{
	return parser.getTicksPerSecond(animName);
}

unsigned int SkeletalModel::getDuration(const char * animName)
{
	return parser.getDuration(animName);
}

unsigned int SkeletalModel::getMaxAffectBones()
{
	return parser.getMaxAffectBones();
}

unsigned int SkeletalModel::getMaxBones(const char *animName)
{
	return parser.getMaxBones(animName);
}

void SkeletalModel::getAnimation(const char * name, SkeletalAnimation & anim)
{
	parser.getAnimation(name, anim);
}
void SkeletalModel::getAnimations(std::map<std::string, SkeletalAnimation>& anims)
{
	parser.getAnimations(anims);
}