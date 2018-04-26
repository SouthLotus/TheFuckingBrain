#ifndef __lowpoly_terrain
#define __lowpoly_terrain
#include <gl\glew.h>
#include <string>
#include <glm\glm.hpp>
#include "..\shader\LowPolyShader.hpp"
#include "..\owner\GLVAOOwner.hpp"
#include "..\owner\GLVBOOwner.hpp"
#include "..\object\Camera.hpp"
#include "..\object\DirectionalLight.hpp"
#include "..\shader\TerrainShader.hpp"
#include "..\tool\LowPolyTerrainMap.hpp"
class LowPolyTerrain {
private:
	GLVAOOwner vaoOwner;
	GLVBOOwner vboOwner;
	unsigned int numVer;
	TerrainShader shader;
	LowPolyTerrainMap terrainMap;
	void initVao();
public:
	LowPolyTerrain() = default;
	~LowPolyTerrain() = default;
	void render(Camera &camera, DirectionalLight &lit);
	void init();
	float getHeight(float x, float z);
};
#endif