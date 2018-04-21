#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "DiamondSquareTerrain.hpp"
#include <cstdlib>
#include <ctime>
#include <memory>
#include <cstdio>
#include <iostream>
#include <stb\stb_image_write.h>
#include <glm\glm.hpp>
#define RAND random(-max, max)

DiamondSquareTerrain::DiamondSquareTerrain(int level, float max) :
	level(level), max(max), size((1 << level) + 1)
{
	data.resize(size);
	for (int i = 0; i < size; i++) {
		data[i].resize(size, -1);
	}
	build();
}

void DiamondSquareTerrain::genRandom()
{
	std::srand(std::time(nullptr));
}

float DiamondSquareTerrain::random(float from, float to)
{
	return ((float)std::rand() / RAND_MAX) * (to - from) + from;
}

void DiamondSquareTerrain::diamond(
	int offsetX, int offsetY, int sizem1, int randFactor) {
	int reducer = 1 << randFactor;
	int halfSize = sizem1 / 2;
	float tl = data[offsetY][offsetX];
	float tr = data[offsetY][offsetX + sizem1];
	float br = data[offsetY + sizem1][offsetX + sizem1];
	float bl = data[offsetY + sizem1][offsetX];
	float midValue = ((tl + tr + br + bl) / 4) + (RAND / reducer);
	if (midValue > max) {
		midValue = max;
	}
	else if (midValue < 0) {
		midValue = 0;
	}
	data[offsetY + halfSize][offsetX + halfSize] = midValue;
}

void DiamondSquareTerrain::square(
	int offsetX, int offsetY, int sizem1, int randFactor)
{
	int reduce = 1 << randFactor;
	int halfSize = sizem1 / 2;
	float tl = data[offsetY][offsetX];
	float tr = data[offsetY][offsetX + sizem1];
	float br = data[offsetY + sizem1][offsetX + sizem1];
	float bl = data[offsetY + sizem1][offsetX];
	float mid = data[offsetY + halfSize][offsetX + halfSize];
	//left
	/*
	0****
	*****
	0*0**
	*****
	0****
	*/
	float &midLeft = data[offsetY + halfSize][offsetX];
	if (midLeft == -1) {
		int leftDiamond = offsetX - halfSize;
		if (leftDiamond < 0 ||
			data[offsetY + halfSize][leftDiamond] == -1) {
			midLeft = ((tl + bl + mid) / 3) + (RAND / reduce);
		}
		else {
			midLeft =
				((tl + bl + mid + data[offsetY + halfSize][leftDiamond]) / 4)
				+ (RAND / reduce);
		}
		if (midLeft > max) {
			midLeft = max;
		}
		else if (midLeft < 0) {
			midLeft = 0;
		}
	}
	//top
	/*
	0*0*0
	*****
	**0**
	*****
	*****
	*/
	float &midTop = data[offsetY][offsetX + halfSize];
	if (midTop == -1) {
		int topDiamond = offsetY - halfSize;
		if (topDiamond < 0 ||
			data[topDiamond][offsetX + halfSize] == -1) {
			midTop = ((tl + tr + mid) / 3) + (RAND / reduce);
		}
		else {
			midTop =
				((tl + tr + mid + data[topDiamond][offsetX + halfSize]) / 4)
				+ (RAND / reduce);
		}
		if (midTop > max) {
			midTop = max;
		}
		else if (midTop < 0) {
			midTop = 0;
		}
	}
	//right

	/*
	****0
	*****
	**0*0
	*****
	****0
	*/
	float &midRight = data[offsetY + halfSize][offsetX + sizem1];
	if (midRight == -1) {
		int rightDiamond = offsetX + sizem1 + halfSize;
		if (rightDiamond >= sizem1 ||
			data[offsetY + halfSize][rightDiamond] == -1) {
			midRight = ((tr + br + mid) / 3) + (RAND / reduce);
		}
		else {
			midRight =
				((tr + br + mid + data[offsetY + halfSize][rightDiamond]) / 4)
				+ (RAND / reduce);
		}
		if (midRight > max) {
			midRight = max;
		}
		else if (midRight < 0) {
			midRight = 0;
		}
	}
	//bot
	/*
	*****
	*****
	**0**
	*****
	0*0*0
	*/
	float &midBottom = data[offsetY + sizem1][offsetX + halfSize];
	if (midBottom == -1) {
		int botDiamond = offsetY + sizem1 + halfSize;
		if (botDiamond >= sizem1 || 
			data[botDiamond][offsetX + halfSize] == -1) {
			midBottom = ((bl + br + mid) / 3) + (RAND / reduce);
		}
		else {
			midBottom =
				((bl + br + mid + data[botDiamond][offsetX + halfSize]) / 4)
				+ (RAND / reduce);
		}
		if (midBottom > max) {
			midBottom = max;
		}
		else if (midBottom < 0) {
			midBottom = 0;
		}
	}
}

void DiamondSquareTerrain::processDiamondSquare(
	int offsetX, int offsetY, int sizem1, int randFactor)
{
	diamond(offsetX, offsetY, sizem1, randFactor);
	square(offsetX, offsetY, sizem1, randFactor);
	sizem1 /= 2;
	if (sizem1 == 1) {
		return;
	}
	randFactor++;
	int tmpOffsetX = offsetX + sizem1;
	int tmpOffsetY = offsetY + sizem1;
	processDiamondSquare(offsetX, offsetY, sizem1, randFactor);
	processDiamondSquare(tmpOffsetX, offsetY, sizem1, randFactor);
	processDiamondSquare(tmpOffsetX, tmpOffsetY, sizem1, randFactor);
	processDiamondSquare(offsetX, tmpOffsetY, sizem1, randFactor);
}

void DiamondSquareTerrain::build()
{
	int sizem1 = size - 1;
	genRandom();
	data[0][0] = random(0, max);
	data[0][sizem1] = random(0, max);
	data[sizem1][sizem1] = random(0, max);
	data[sizem1][0] = random(0, max);
	processDiamondSquare(0, 0, sizem1, 0);
}

void DiamondSquareTerrain::genTriangleMesh(
	int rowIndex, int colIndex, 
	int terrainStride, float meshStride, 
	std::vector<glm::vec3> &mesh, std::vector<glm::vec3> &normal)
{
	int offsetX = colIndex * terrainStride;
	int offsetY = rowIndex * terrainStride;
	/*
	tl 0 0 0 tr
	 0 0 0 0 0
	 0 0 0 0 0
	 0 0 0 0 0
	bl 0 0 0 br
	*/
	//top left
	glm::vec3 tl(
		colIndex * meshStride, data[offsetY][offsetX], rowIndex * meshStride);
	//top right
	glm::vec3 tr(
		tl.x + meshStride, data[offsetY][offsetX + terrainStride], tl.z);
	//bottom right
	glm::vec3 br(
		tr.x, data[offsetY + terrainStride][offsetX + terrainStride], tl.z + meshStride);
	//bottom left
	glm::vec3 bl(
		tl.x, data[offsetY + terrainStride][offsetX], br.z);
	// counter clock wise
	//top Triagle
	//normal
	glm::vec3 tv1 = bl - tl;
	glm::vec3 tv2 = tr - tl;
	glm::vec3 tnormal = glm::normalize(glm::cross(tv1, tv2));
	mesh.push_back(tl);
	mesh.push_back(bl);
	mesh.push_back(tr);
	normal.push_back(tnormal);
	normal.push_back(tnormal);
	normal.push_back(tnormal);
	//bottom Triagle
	//normal
	glm::vec3 bv1 = bl - tr;
	glm::vec3 bv2 = br - tr;
	glm::vec3 bnormal = glm::normalize(glm::cross(bv1, bv2));
	mesh.push_back(tr);
	mesh.push_back(bl);
	mesh.push_back(br);
	normal.push_back(bnormal);
	normal.push_back(bnormal);
	normal.push_back(bnormal);
}

void DiamondSquareTerrain::genQuadMesh(int rowIndex, int colIndex, int terrainStride, float meshStride, std::vector<float>& mesh)
{
	int offsetX = colIndex * terrainStride;
	int offsetY = rowIndex * terrainStride;
	/*
	tl 0 0 0 tr
	0 0 0 0 0
	0 0 0 0 0
	0 0 0 0 0
	bl 0 0 0 br
	*/
	//Clock wise
	//top left
	float tlX = colIndex * meshStride;
	float tlY = rowIndex * meshStride;
	float tlZ = data[offsetY][offsetX];
	//top right
	float trX = tlX + meshStride;
	float trY = tlY;
	float trZ = data[offsetY][offsetX + terrainStride];
	//bottom right
	float brX = trX;
	float brY = tlY + meshStride;
	float brZ =
		data[offsetY + terrainStride][offsetX + terrainStride];
	//bottom left
	std::cout << brZ << std::endl;
	float blX = tlX;
	float blY = brY;
	float blZ = data[offsetY + terrainStride][offsetX];
	
	//Quad
	mesh.push_back(tlX);
	mesh.push_back(tlY);
	mesh.push_back(tlZ);
	mesh.push_back(blX);
	mesh.push_back(blY);
	mesh.push_back(blZ);
	mesh.push_back(brX);
	mesh.push_back(brY);
	mesh.push_back(brZ);
	mesh.push_back(trX);
	mesh.push_back(trY);
	mesh.push_back(trZ);
}

std::vector<float> DiamondSquareTerrain::toRGBf()
{
	std::vector<float> rgb(size * size * 3);
	int rgbIndex = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			float colorValue = data[i][j] * mul;
			rgb[rgbIndex] = colorValue;
			rgb[rgbIndex + 1] = colorValue;
			rgb[rgbIndex + 2] = colorValue;
			rgbIndex += 3;
		}
	}
	return rgb;
}

std::vector<dst_uc> DiamondSquareTerrain::toRGBuc()
{
	std::vector<dst_uc> rgb(size * size * 3);
	int rgbIndex = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			dst_uc colorValue = (dst_uc)data[i][j] * mul;
			rgb[rgbIndex] = colorValue;
			rgb[rgbIndex + 1] = colorValue;
			rgb[rgbIndex + 2] = colorValue;
			rgbIndex += 3;
		}
	}
	return rgb;
}

void DiamondSquareTerrain::toTriangleMesh(
	float meshSize, int level, std::vector<glm::vec3> &mesh,
	std::vector<glm::vec3> &normal
) {
	int numPart = 1 << level;
	int terrainStride = (size - 1) / numPart;
	float meshStride = meshSize / numPart;
	for (int i = 0; i < numPart; i++) {
		for (int j = 0; j < numPart; j++) {
			genTriangleMesh(i, j, terrainStride, meshStride, mesh, normal);
		}
	}
}

std::vector<float> DiamondSquareTerrain::toQuadMesh(
	float meshSize, int level
) {
	std::vector<float> mesh;
	int numPart = 1 << level;
	int terrainStride = (size - 1) / numPart;
	float meshStride = meshSize / numPart;

	for (int i = 0; i < numPart; i++) {
		for (int j = 0; j < numPart; j++) {
			genQuadMesh(i, j, terrainStride, meshStride, mesh);
		}
	}
	return mesh;
}

void DiamondSquareTerrain::print()
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			std::printf("%6.2f ", data[i][j]);
		}
		std::printf("\n");
	}
}

/*void DiamondSquareTerrain::printTriangleMesh()
{
	std::vector<float> mesh = toTriangleMesh(size, level);
	int size = mesh.size();
	for (int i = 0; i < size; i++) {
		std::printf("%6.2f ", mesh[i]);
	}
}*/

void DiamondSquareTerrain::saveTGA(const char *path) {
	std::vector<dst_uc> rgb(size * size);
	int rgbIndex = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			rgb[rgbIndex] = (dst_uc)data[i][j] * mul;
			rgbIndex ++;
		}
	}
	stbi_write_tga(path, size, size, 1, rgb.data());
}

void DiamondSquareTerrain::saveBMP(const char * path) {
	std::vector<dst_uc> dataSmPtr = toRGBuc();
	stbi_write_bmp(path, size, size, 3, dataSmPtr.data());
}

void DiamondSquareTerrain::multiply(float num) {
	mul = num;
}