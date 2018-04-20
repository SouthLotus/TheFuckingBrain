#ifndef __math_tool
#define __math_tool
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
namespace matht {
	glm::mat3 changeBasis(
		glm::vec3 nxb, glm::vec3 nyb, glm::vec3 nzb);
	glm::mat3 thisToAnother(
		glm::vec3 nxb, glm::vec3 nyb, glm::vec3 nzb);
}
#endif