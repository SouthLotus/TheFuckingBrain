#include "Model.hpp"
#include "..\tool\ModelTool.hpp"
#include <vector>
#include <assimp\scene.h>

Model::Model(const char *filename):
tool(filename)
{
}

const std::vector<float>* Model::getCoords()
{
	return &coords;
}

const std::vector<float>* Model::getNormals()
{
	return &normals;
}

const std::vector<float>* Model::getTexCoords()
{
	return &texCoords;
}

const std::vector<float>* Model::getKAS()
{
	return &kas;
}

const std::vector<float>* Model::getKDS()
{
	return &kds;
}

const std::vector<float>* Model::getKSS()
{
	return &kss;
}

void Model::saveCoords()
{
	tool.loadModelC(coords);
}

void Model::saveNormals()
{
	tool.loadModelN(normals);
}

void Model::saveTexCoords()
{
	tool.loadModelT(texCoords);
}

void Model::saveKAS(int numElements)
{
	if (numElements == 3) {
		tool.loadModelM3V(kas, AI_MATKEY_COLOR_AMBIENT);
	}
	else if (numElements == 4) {
		tool.loadModelM4V(kas, AI_MATKEY_COLOR_AMBIENT);
	}
}

void Model::saveKDS(int numElements)
{
	if (numElements == 3) {
		tool.loadModelM3V(kds, AI_MATKEY_COLOR_DIFFUSE);
	}
	else if (numElements == 4) {
		tool.loadModelM4V(kds, AI_MATKEY_COLOR_DIFFUSE);
	}
}

void Model::saveKSS(int numElements)
{
	if (numElements == 3) {
		tool.loadModelM3V(kss, AI_MATKEY_COLOR_SPECULAR);
	}
	else if (numElements == 4) {
		tool.loadModelM4V(kss, AI_MATKEY_COLOR_SPECULAR);
	}
}
