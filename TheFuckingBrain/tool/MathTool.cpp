#include "MathTool.hpp"
#include <vector>
#include <cstdio>
#include <glm\glm.hpp>

glm::mat3 matht::changeBasis(glm::vec3 nxb, glm::vec3 nyb, glm::vec3 nzb)
{
	nxb = glm::normalize(nxb);
	nyb = glm::normalize(nyb);
	nzb = glm::normalize(nzb);
	return glm::mat3(
		glm::vec3(nxb.x, nxb.x, nzb.x),
		glm::vec3(nxb.y, nyb.y, nzb.y),
		glm::vec3(nxb.z, nyb.z, nzb.z)
	);
}

glm::mat3 matht::basisAToB(glm::vec3 axInB, glm::vec3 ayInB, glm::vec3 azInB)
{
	axInB = glm::normalize(axInB);
	ayInB = glm::normalize(ayInB);
	azInB = glm::normalize(azInB);
	return glm::mat3(
		axInB,
		ayInB,
		azInB
	);
}

double matht::GaussianDistr2D(double x, double y, double dev) {
	double squareDevMul2 = 2 * dev * dev;
	return glm::exp(-(x*x + y*y)/squareDevMul2) / (squareDevMul2 * glm::pi<double>());
}

void matht::GaussianBlurMatrix(int level, double dev,
	std::vector<std::vector<double>> &matrix) {
	int size = 2 * level + 1;
	matrix.resize(size);
	int limit = (size - 1) / 2;
	double sum = 0;
	for (int i = 0; i < size; i++) {
		int y = -i + limit;
		matrix[i].resize(size);
		for (int j = 0; j < size; j++) {
			int x = j - limit;
			matrix[i][j] = GaussianDistr2D(x, y, dev);
			sum += matrix[i][j];
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			matrix[i][j] /= sum;
		}
	}
}
