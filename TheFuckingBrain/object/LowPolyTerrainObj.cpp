#include "LowPolyTerrainObj.hpp"
#include "..\model\LowPolyTerrain.hpp"
#include <memory>

void LowPolyTerrainObj::setSkin(
std::shared_ptr<LowPolyTerrain> terrainSkin) {
	this->terrainSkin = terrainSkin;
}

void LowPolyTerrainObj::show()
{
	//terrainSkin->render();
}