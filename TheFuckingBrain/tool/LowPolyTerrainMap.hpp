#ifndef __terrain_map
#define __terrain_map
#include <glm\glm.hpp>
#include <vector>
class LowPolyTerrainMap {
private:
	float size;
	float stride;
	std::vector<std::vector<float>> heights;
	//method
	void genTriangleMesh(
		int rowIndex, int colIndex,
		std::vector<glm::vec3> &mesh,
		std::vector<glm::vec3> &normal);
	void genQuadMesh(
		int rowIndex, int colIndex,
		std::vector<glm::vec3> &mesh,
		std::vector<glm::vec3> &normal);
	glm::vec3 getVertex(int row, int col);
public:
	LowPolyTerrainMap() = default;
	LowPolyTerrainMap(
		std::vector<std::vector<float>> heights,
		float size);
	~LowPolyTerrainMap() = default;
	void toTriangleMesh(
		std::vector<glm::vec3> &mesh,
		std::vector<glm::vec3> &normal
	);
	void toQuadMesh(
		std::vector<glm::vec3> &mesh,
		std::vector<glm::vec3> &normal
	);
	float getHeight(float x, float z);
	float getStride();
};
#endif