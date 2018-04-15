#ifndef __sky_box
#define __sky_box
#include <gl\glew.h>
#include <string>
#include "..\shader\SkyBoxShader.hpp"
#include "..\owner\GLVAOOwner.hpp"
#include "..\owner\GLVBOOwner.hpp"
#include "..\owner\GLTextureOwner.hpp"
#include "..\object\Camera.hpp"
class SkyBox {
private:
	GLVAOOwner vaoOwner;
	GLVBOOwner vboOwner;
	GLTextureOwner texOwner;
	unsigned int numVer;
	SkyBoxShader shader;
	void initVao(std::string modelPath);
	void initTexture(std::string *texturePath);
public:
	SkyBox();
	~SkyBox();
	void render(Camera &camera);
	void init(std::string modelPath,
		std::string *texturePath);
};
#endif