#ifndef __sky_box_shader
#define __sky_box_shader
#include "..\owner\GLProgramOwner.hpp"
class SkyBoxShader {
private:
	const static char *VTSD_PATH;
	const static char *FRSD_PATH;
	GLProgramOwner programOwner;
	int iVCoord;
	int uProjModelView;
	int uSampler;
	bool isInit = false;
public:
	SkyBoxShader() = default;
	~SkyBoxShader() = default;
	void init();
	void useProgram();
	void unUseProgram();
	int getIVCoord() const;
	int getUProjModelView() const;
	int getUSampler() const;
};
#endif