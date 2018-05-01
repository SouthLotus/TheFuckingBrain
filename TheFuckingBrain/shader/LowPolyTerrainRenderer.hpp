#ifndef __terrain_renderer
#define __terrain_renderer
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
class LowPolyTerrainRenderer {
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
	LowPolyTerrainRenderer() = default;
	~LowPolyTerrainRenderer() = default;
	void render(Camera &camera, DirectionalLight &lit);
	void init();
	float getHeight(float x, float z);
};
#endif