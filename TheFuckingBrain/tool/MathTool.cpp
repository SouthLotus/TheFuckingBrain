#include "MathTool.hpp"

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

glm::mat3 matht::thisToAnother(glm::vec3 nxb, glm::vec3 nyb, glm::vec3 nzb)
{
	nxb = glm::normalize(nxb);
	nyb = glm::normalize(nyb);
	nzb = glm::normalize(nzb);
	return glm::mat3(
		nxb,
		nyb,
		nzb
	);
}
