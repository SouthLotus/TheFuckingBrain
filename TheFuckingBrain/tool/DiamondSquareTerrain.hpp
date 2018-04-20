#ifndef __diamond_square_terrain
#define __diamond_square_terrain
#include <vector>
#include <glm\glm.hpp>
typedef unsigned char dst_uc;
class DiamondSquareTerrain {
private:
	std::vector<std::vector<float>> data;
	int size;
	int level;
	float max;
	float mul = 1;
	void genRandom();
	float random(float from, float to);
	void diamond(
		int offsetX, int offsetY,
		int sizem1, int randFactor);
	void square(
		int offsetX, int offsetY,
		int sizem1, int randFactor);
	void processDiamondSquare(
		int offsetX, int offsetY,
		int sizem1, int randFactor);
	void build();
	void genTriangleMesh(
		int rowIndex, int colIndex,
		int terrainStride, float meshStride,
		std::vector<glm::vec3> &mesh,
		std::vector<glm::vec3> &normal);
	void genQuadMesh(
		int rowIndex, int colIndex,
		int terrainStride, float meshStride,
		std::vector<float> &mesh);
public:
	DiamondSquareTerrain(int level, float max);
	~DiamondSquareTerrain() = default;
	void saveTGA(const char *path);
	void saveBMP(const char *path);
	void multiply(float num);
	std::vector<float> toRGBf();
	std::vector<dst_uc> toRGBuc();
	void toTriangleMesh(
		float meshSize, int level,
		std::vector<glm::vec3> &mesh,
		std::vector<glm::vec3> &normal
	);
	std::vector<float> toQuadMesh(
		float meshSize, int level
	);

	void print();
	/*void printTriangleMesh();*/
};
#endif
/*
level   value
0        1
1        2
2        4
3        8
4        16
5        32
6        64
7        128
8        256
9        512
10       1024
11       2048
12       4096
13       8192
*/