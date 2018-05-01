#include "LowPolyTerrainMap.hpp"
#include <vector>
#include <glm\glm.hpp>
#include <cmath>
#include "..\owner\FileOwner.hpp"
#include <fstream>

void LowPolyTerrainMap::genTriangleMesh(
	int rowIndex, int colIndex, std::vector<glm::vec3>& mesh, std::vector<glm::vec3>& normal)
{
	float offsetX = colIndex * stride;
	float offsetZ = rowIndex * stride;
	/*
	tl 0 0 0 tr
	0 0 0 0 0
	0 0 0 0 0
	0 0 0 0 0
	bl 0 0 0 br
	*/
	//top left
	glm::vec3 tl(
		offsetX, heights[rowIndex][colIndex], offsetZ);
	//top right
	glm::vec3 tr(
		tl.x + stride, heights[rowIndex][colIndex + 1], tl.z);
	//bottom right
	glm::vec3 br(
		tr.x, heights[rowIndex + 1][colIndex + 1], tl.z + stride);
	//bottom left
	glm::vec3 bl(
		tl.x, heights[rowIndex + 1][colIndex], br.z);
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

void LowPolyTerrainMap::genQuadMesh(
	int rowIndex, int colIndex, std::vector<glm::vec3>& mesh, std::vector<glm::vec3>& normal)
{
	int offsetX = colIndex * stride;
	int offsetZ = rowIndex * stride;
	/*
	tl 0 0 0 tr
	0 0 0 0 0
	0 0 0 0 0
	0 0 0 0 0
	bl 0 0 0 br
	*/
	//top left
	glm::vec3 tl(
		offsetX, heights[rowIndex][colIndex], offsetZ);
	//top right
	glm::vec3 tr(
		tl.x + stride, heights[rowIndex][colIndex + 1], tl.z);
	//bottom right
	glm::vec3 br(
		tr.x, heights[rowIndex + 1][colIndex + 1], tl.z + stride);
	//bottom left
	glm::vec3 bl(
		tl.x, heights[rowIndex + 1][colIndex], br.z);
	// counter clock wise
	//top Triagle
	//normal
	glm::vec3 tv1 = bl - tl;
	glm::vec3 tv2 = tr - tl;
	glm::vec3 tnormal = glm::normalize(glm::cross(tv1, tv2));
	mesh.push_back(tl);
	mesh.push_back(bl);
	mesh.push_back(br);
	mesh.push_back(tr);
}

glm::vec3 LowPolyTerrainMap::getVertex(int row, int col)
{
	
	return glm::vec3(col * stride, heights[row][col], row * stride);
}

LowPolyTerrainMap::LowPolyTerrainMap(
	std::vector<std::vector<float>> heights,
	float size):
	heights(heights), size(size), stride(size / (heights.size() - 1))
{
}

LowPolyTerrainMap::LowPolyTerrainMap(const char * filename)
{
	FileOwner file(filename, 
		std::ios_base::in | std::ios_base::binary);
	int numVerticesOnEdge = 0;
	file->read((char *)&size, sizeof(float));
	file->read((char *)&numVerticesOnEdge, sizeof(int));
	stride = size / (numVerticesOnEdge - 1);
	heights.resize(numVerticesOnEdge);
	for (int i = 0; i < numVerticesOnEdge; i++) {
		heights[i].resize(numVerticesOnEdge);
		for (int j = 0; j < numVerticesOnEdge; j++) {
			file->read(
				(char *)&heights[i][j], sizeof(float));
		}
	}
}

void LowPolyTerrainMap::toTriangleMesh(
	std::vector<glm::vec3>& mesh, std::vector<glm::vec3>& normal)
{
	int numParts = heights.size() - 1;
	for (int i = 0; i < numParts; i++) {
		for (int j = 0; j < numParts; j++) {
			genTriangleMesh(i, j, mesh, normal);
		}
	}
}

void LowPolyTerrainMap::toQuadMesh(
	std::vector<glm::vec3>& mesh, std::vector<glm::vec3>& normal)
{
	int numParts = heights.size() - 1;
	for (int i = 0; i < numParts; i++) {
		for (int j = 0; j < numParts; j++) {
			genQuadMesh(i, j, mesh, normal);
		}
	}
}

float LowPolyTerrainMap::getHeight(float x, float z)
{
	int colIndex = x / stride;
	int rowIndex = z / stride;
	glm::vec3 tl = getVertex(rowIndex, colIndex);
	glm::vec3 bl = getVertex(rowIndex + 1, colIndex);
	glm::vec3 br = getVertex(rowIndex + 1, colIndex + 1);
	glm::vec3 tr = getVertex(rowIndex, colIndex + 1);
	float dx = std::fmod(x, stride);
	float dz = std::fmod(z, stride);
	if (dx + dz <= stride) {
		glm::vec3 bv1 = bl - tl;
		glm::vec3 bv2 = tr - tl;
		glm::vec3 bv3(x - tl.x, 0, z - tl.z);
		glm::vec3 normal = glm::cross(bv1, bv2);
		return tl.y + (-normal.x / normal.y) * bv3.x + (-normal.z / normal.y) * bv3.z;
	} 
	else {
		glm::vec3 bv1 = tr - br;
		glm::vec3 bv2 = bl - br;
		glm::vec3 bv3(x - br.x, 0, z - br.z);
		glm::vec3 normal = glm::cross(bv1, bv2);
		return br.y + (-normal.x / normal.y) * bv3.x + (-normal.z / normal.y) * bv3.z;
	}
}

float LowPolyTerrainMap::getStride() {
	return stride;
}

void LowPolyTerrainMap::saveToFile(const char * path)
{
	FileOwner file(path, 
		std::ios_base::out|std::ios_base::binary);
	int numVerticesOnEdge = heights.size();
	file->write((char *)&size, sizeof(float));
	file->write((char *)&numVerticesOnEdge, sizeof(int));
	for (int i = 0; i < numVerticesOnEdge; i++) {
		for (int j = 0; j < numVerticesOnEdge; j++) {
			file->write(
				(char *)&heights[i][j], sizeof(float));
		}
	}
}

float LowPolyTerrainMap::getSize() {
	return size;
}


