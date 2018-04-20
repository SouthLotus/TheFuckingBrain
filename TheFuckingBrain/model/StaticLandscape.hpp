#ifndef __static_landscape
#define __static_landscape
#include <gl\glew.h>
#include <string>
#include "..\shader\LowPolyShader.hpp"
#include "..\owner\GLVAOOwner.hpp"
#include "..\owner\GLVBOOwner.hpp"
#include "..\object\Camera.hpp"
#include "..\object\DirectionalLight.hpp"

class StaticLandscape {
private:
	GLVAOOwner vaoOwner;
	GLVBOOwner vboOwner;
	unsigned int numVer;
	LowPolyShader shader;
	void initVao(std::string modelPath);
public:
	StaticLandscape() = default;
	~StaticLandscape() = default;
	void render(Camera &camera, DirectionalLight &lit);
	void init(std::string modelPath);
};
#endif
