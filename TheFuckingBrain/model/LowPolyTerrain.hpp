#ifndef __lowpoly_terrain
#define __lowpoly_terrain
#include <gl\glew.h>
#include <string>
#include <glm\glm.hpp>
#include "..\shader\LowPolyShader.hpp"
#include "..\owner\GLVAOOwner.hpp"
#include "..\owner\GLVBOOwner.hpp"
#include "..\owner\GLTextureOwner.hpp"
#include "..\object\Camera.hpp"
#include "..\object\DirectionalLight.hpp"
#include "..\shader\LowPolyTerrainShader.hpp"
#include "..\tool\LowPolyTerrainMap.hpp"
class LowPolyTerrain {
private:
	GLVAOOwner vaoOwner;
	GLVBOOwner vboOwner;
	GLTextureOwner texOwner;
	unsigned int numVer;
	LowPolyTerrainShader shader;
	LowPolyTerrainMap terrainMap;
	void initVao();
	void initTexture();
public:
	LowPolyTerrain() = default;
	~LowPolyTerrain() = default;
	void render(Camera &camera, DirectionalLight &lit);
	void init();
	float getHeight(float x, float z);
};
#endif