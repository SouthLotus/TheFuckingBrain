#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight(
	glm::vec3 direction, glm::vec3 color, float intensity):
	direction(glm::normalize(direction)), color(color), intensity(intensity)
{
}

void DirectionalLight::setDirection(glm::vec3 direction)
{
	this->direction = glm::normalize(direction);
}

void DirectionalLight::setColor(glm::vec3 color)
{
	this->color = color;
}

void DirectionalLight::setIntensity(float intensity)
{
	this->intensity = intensity;
}

const glm::vec3 * DirectionalLight::getDirection() const
{
	return &direction;
}

const glm::vec3 * DirectionalLight::getColor() const
{
	return &color;
}

float DirectionalLight::getIntensity() const
{
	return intensity;
}
