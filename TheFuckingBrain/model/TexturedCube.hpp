#ifndef __textured_cube
#define __textured_cube
#include <gl\glew.h>
#include <string>
#include "..\shader\BasicShader.hpp"
#include "..\owner\GLVAOOwner.hpp"
#include "..\owner\GLVBOOwner.hpp"
#include "..\owner\GLTextureOwner.hpp"
#include "..\object\Camera.hpp"
#include "..\object\DirectionalLight.hpp"

class TexturedCube {
private:
	GLVAOOwner vaoOwner;
	GLVBOOwner vboOwner;
	GLTextureOwner texOwner;
	unsigned int numVer;
	BasicShader shader;
	void initVao(std::string modelPath);
	void initTexture(std::string texturePath);
public:
	TexturedCube();
	~TexturedCube();
	void render(Camera &camera, DirectionalLight &lit);
	void init(std::string modelPath,
		std::string texturePath);
};
#endif