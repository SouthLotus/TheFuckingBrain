#ifndef __model
#define __model
#include <vector>
#include "..\tool\MyTool.hpp"
class Model{
private:
	std::vector<float> coords;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<float> kas;
	std::vector<float> kds;
	std::vector<float> kss;
	ModelTool tool;
public:
	Model(const char *filename);
	Model() = default;
	~Model() = default;
	const std::vector<float> * getCoords();
	const std::vector<float> * getNormals();
	const std::vector<float> * getTexCoords();
	const std::vector<float> * getKAS();
	const std::vector<float> * getKDS();
	const std::vector<float> * getKSS();
	void saveCoords();
	void saveNormals();
	void saveTexCoords();
	void saveKAS(int numElements);
	void saveKDS(int numElements);
	void saveKSS(int numElements);
};
#endif