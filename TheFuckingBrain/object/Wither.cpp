#include "Wither.hpp"

void Wither::show(Camera & camera, DirectionalLight & dLight)
{
	graphic.render(camera, dLight);
}

void Wither::standOnTerrain(LowPolyTerrain & terrain)
{
	position.y = terrain.getHeight(position.x, position.z);
	graphic.setTranslate(position);
}

void Wither::moveAbsolute(glm::vec3 vec)
{
	position = vec;
	graphic.setTranslate(vec);
}

void Wither::walk()
{
}

void Wither::handBeat()
{
}

void Wither::bulletBeat()
{
}

void Wither::stand()
{
}
