#ifndef __basicshader
#define __basicshader
#include "..\owner\GLProgramOwner.hpp"
class BasicShader {
private:
	const static char *VTSD_PATH;
	const static char *FRSD_PATH;
	GLProgramOwner programOwner;
	int iVCoord;
	int iVNormal;
	int iVTexCoord;
	int uProjModelView;
	int uSampler;
	int uLitColor;
	int uLitDirection;
	int uLitIntensity;
	int uCameraPosition;
public:
	BasicShader() = default;
	~BasicShader() = default;
	void init();
	void useProgram();
	void unUseProgram();
	int getIVCoord() const;
	int getIVNormal() const;
	int getIVTexCoord() const;
	int getUProjModelView() const;
	int getUSampler() const;
	int getULitColor() const;
	int getULitDirection() const;
	int getULitIntensity() const;
	int getUCameraPosition() const;
};
#endif