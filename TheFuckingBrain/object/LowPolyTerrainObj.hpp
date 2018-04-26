#ifndef __lowpoly_terrain_obj
#define __lowpoly_terrain_obj
#include "..\model\LowPolyTerrain.hpp"
#include <memory>
class LowPolyTerrainObj {
private:
	std::shared_ptr<LowPolyTerrain> terrainSkin;
public:
	LowPolyTerrainObj() = default;
	~LowPolyTerrainObj() = default;
	void setSkin(std::shared_ptr<LowPolyTerrain> terrainSkin);
	void show();
};
#endif