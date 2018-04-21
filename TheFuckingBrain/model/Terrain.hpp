#ifndef __terrain
#define __terrain
#include <gl\glew.h>
#include <string>
#include "..\shader\LowPolyShader.hpp"
#include "..\owner\GLVAOOwner.hpp"
#include "..\owner\GLVBOOwner.hpp"
#include "..\object\Camera.hpp"
#include "..\object\DirectionalLight.hpp"
#include "..\shader\TerrainShader.hpp"

class Terrain {
private:
	GLVAOOwner vaoOwner;
	GLVBOOwner vboOwner;
	unsigned int numVer;
	TerrainShader shader;
	void initVao();
public:
	Terrain() = default;
	~Terrain() = default;
	void render(Camera &camera, DirectionalLight &lit);
	void init();
};
#endif
