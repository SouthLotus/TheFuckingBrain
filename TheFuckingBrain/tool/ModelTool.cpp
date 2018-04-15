#include "ModelTool.hpp"
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <string>
#include <iostream>
#include <exception>
#include <vector>

ModelTool::ModelTool(std::string path) {
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate);
	if (scene == nullptr) {
		throw std::runtime_error("ModelTool::ModelTool() exception.");
	}
}

ModelTool::~ModelTool() {
	
}

/*Load model chi lay vertex Coordinates*/
void ModelTool::loadModelC(
	std::vector<float> &coordVector)
{
	const aiScene *scene = this->importer.GetScene();
	aiMesh **meshes = scene->mMeshes;
	int numMeshes = scene->mNumMeshes;
	/*Luu tru cac coord cua vertex trong vector<float>*/
	for (int meshIndex = 0; meshIndex < numMeshes; meshIndex++) {
		aiMesh *mesh = meshes[meshIndex];
		aiFace *faces = mesh->mFaces;
		aiVector3D *coords = mesh->mVertices;
		int numFaces = mesh->mNumFaces;
		for (int faceIndex = 0; faceIndex < numFaces; faceIndex++) {
			aiFace face = faces[faceIndex];
			unsigned int *vertexIndicesInFace = face.mIndices;
			int numVertexIndices = face.mNumIndices;
			for (int vertexIndex = 0; vertexIndex < numVertexIndices; vertexIndex++) {
				aiVector3D coord = coords[vertexIndicesInFace[vertexIndex]];
				coordVector.push_back(coord.x);
				coordVector.push_back(coord.y);
				coordVector.push_back(coord.z);
			}
		}
	}
}

/*Load model lay vertex Coordinates, Normals, Texture coordinates*/
void ModelTool::loadModelCNT( 
	std::vector<float>& coordVector, 
	std::vector<float>& normalVector, 
	std::vector<float>& texVector)
{
	const aiScene *scene = this->importer.GetScene();
	aiMesh **meshes = scene->mMeshes;
	int numMeshes = scene->mNumMeshes;
	/*Luu tru cac coord, normal cua vertex trong vector<float>*/
	for (int meshIndex = 0; meshIndex < numMeshes; meshIndex++) {
		aiMesh *mesh = meshes[meshIndex];
		aiFace *faces = mesh->mFaces;
		aiVector3D *coords = mesh->mVertices;
		aiVector3D *normals = mesh->mNormals;
		aiVector3D *texCoords = mesh->mTextureCoords[0];
		int numFaces = mesh->mNumFaces;
		for (int faceIndex = 0; faceIndex < numFaces; faceIndex++) {
			aiFace face = faces[faceIndex];
			unsigned int *vertexIndicesInFace = face.mIndices;
			int numVertexIndices = face.mNumIndices;
			for (int vertexIndexInFace = 0; vertexIndexInFace < numVertexIndices; vertexIndexInFace++) {
				int vertexIndex = vertexIndicesInFace[vertexIndexInFace];
				aiVector3D coord = coords[vertexIndex];
				aiVector3D normal = normals[vertexIndex];
				aiVector3D texCoord = texCoords[vertexIndex];
				coordVector.push_back(coord.x);
				coordVector.push_back(coord.y);
				coordVector.push_back(coord.z);

				normalVector.push_back(normal.x);
				normalVector.push_back(normal.y);
				normalVector.push_back(normal.z);

				texVector.push_back(texCoord.x);
				texVector.push_back(texCoord.y);
				texVector.push_back(texCoord.z);
			}
		}
	}
}

/*Load model lay vertex Coordinates, Normals, 2D Texture coordinates*/
void ModelTool::loadModelCNT2D(
	std::vector<float>& coordVector,
	std::vector<float>& normalVector,
	std::vector<float>& texVector)
{
	const aiScene *scene = this->importer.GetScene();
	aiMesh **meshes = scene->mMeshes;
	int numMeshes = scene->mNumMeshes;
	/*Luu tru cac coord, normal cua vertex trong vector<float>*/
	for (int meshIndex = 0; meshIndex < numMeshes; meshIndex++) {
		aiMesh *mesh = meshes[meshIndex];
		aiFace *faces = mesh->mFaces;
		aiVector3D *coords = mesh->mVertices;
		aiVector3D *normals = mesh->mNormals;
		aiVector3D *texCoords = mesh->mTextureCoords[0];
		int numFaces = mesh->mNumFaces;
		for (int faceIndex = 0; faceIndex < numFaces; faceIndex++) {
			aiFace face = faces[faceIndex];
			unsigned int *vertexIndicesInFace = face.mIndices;
			int numVertexIndices = face.mNumIndices;
			for (int vertexIndexInFace = 0; vertexIndexInFace < numVertexIndices; vertexIndexInFace++) {
				int vertexIndex = vertexIndicesInFace[vertexIndexInFace];
				aiVector3D coord = coords[vertexIndex];
				aiVector3D normal = normals[vertexIndex];
				aiVector3D texCoord = texCoords[vertexIndex];
				coordVector.push_back(coord.x);
				coordVector.push_back(coord.y);
				coordVector.push_back(coord.z);

				normalVector.push_back(normal.x);
				normalVector.push_back(normal.y);
				normalVector.push_back(normal.z);

				texVector.push_back(texCoord.x);
				texVector.push_back(texCoord.y);
			}
		}
	}
}

/*Lay material properties cua model co dang AI_MATKEY_COLOR_XXX aiVector3D*/
void ModelTool::loadModelM3V(
	std::vector<float>& propertyValueVector,
	const char * pKey, int type, int idx)
{
	const aiScene *scene = this->importer.GetScene();
	aiMesh **meshes = scene->mMeshes;
	aiMaterial **materials = scene->mMaterials;
	int numMeshes = scene->mNumMeshes;
	/*Luu tru cac coord, normal cua vertex trong vector<float>*/
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
				propertyValueVector.push_back(color.r);
				propertyValueVector.push_back(color.g);
				propertyValueVector.push_back(color.b);
				
			}
		}
	}
}

/*Lay material properties cua model co dang AI_MATKEY_COLOR_XXX aiVector4D*/
void ModelTool::loadModelM4V(
	std::vector<float>& propertyValueVector,
	const char * pKey, int type, int idx)
{
	const aiScene *scene = this->importer.GetScene();
	aiMesh **meshes = scene->mMeshes;
	aiMaterial **materials = scene->mMaterials;
	int numMeshes = scene->mNumMeshes;
	/*Luu tru cac coord, normal cua vertex trong vector<float>*/
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
				propertyValueVector.push_back(color.r);
				propertyValueVector.push_back(color.g);
				propertyValueVector.push_back(color.b);
				propertyValueVector.push_back(color.a);
			}
		}
	}
}

/*Load model lay vertex Coordinates, Normals*/
void ModelTool::loadModelCN(
	std::vector<float> &coordVector,
	std::vector<float> &normalVector)
{
	const aiScene *scene = this->importer.GetScene();
	aiMesh **meshes = scene->mMeshes;
	int numMeshes = scene->mNumMeshes;
	/*Luu tru cac coord, normal cua vertex trong vector<float>*/
	for (int meshIndex = 0; meshIndex < numMeshes; meshIndex++) {
		aiMesh *mesh = meshes[meshIndex];
		aiFace *faces = mesh->mFaces;
		aiVector3D *coords = mesh->mVertices;
		aiVector3D *normals = mesh->mNormals;
		int numFaces = mesh->mNumFaces;
		for (int faceIndex = 0; faceIndex < numFaces; faceIndex++) {
			aiFace face = faces[faceIndex];
			unsigned int *vertexIndicesInFace = face.mIndices;
			int numVertexIndices = face.mNumIndices;
			for (int vertexIndexInFace = 0; vertexIndexInFace < numVertexIndices; vertexIndexInFace++) {
				int vertexIndex = vertexIndicesInFace[vertexIndexInFace];
				aiVector3D coord = coords[vertexIndex];
				aiVector3D normal = normals[vertexIndex];
				coordVector.push_back(coord.x);
				coordVector.push_back(coord.y);
				coordVector.push_back(coord.z);

				normalVector.push_back(normal.x);
				normalVector.push_back(normal.y);
				normalVector.push_back(normal.z);
			}
		}
	}
}
