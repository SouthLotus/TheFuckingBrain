#ifndef __directional_light
#define __directional_light
#include <glm\glm.hpp>
class DirectionalLight {
private:
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
public:
	DirectionalLight() = default;
	DirectionalLight(glm::vec3 direction, 
		glm::vec3 color,
		float intensity);
	~DirectionalLight() = default;
	void setDirection(glm::vec3 direction);
	void setColor(glm::vec3 color);
	void setIntensity(float intensity);
	const glm::vec3 * getDirection() const;
	const glm::vec3 * getColor() const;
	float getIntensity() const;
};
#endif