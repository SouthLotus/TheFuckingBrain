#ifndef __low_poly_shader
#define __low_poly_shader
#include "..\owner\GLProgramOwner.hpp"
class LowPolyShader {
private:
	const static char *VTSD_PATH;
	const static char *FRSD_PATH;
	GLProgramOwner programOwner;
	int iVCoord;
	int iVNormal;
	int ika;
	int ikd;
	int iks;
	int uProjModelView;
	int uLitColor;
	int uLitDirection;
	int uLitIntensity;
	int uCameraPosition;
public:
	LowPolyShader() = default;
	~LowPolyShader() = default;
	void init();
	void useProgram();
	void unUseProgram();
	int getIVCoord() const;
	int getIVNormal() const;
	int getIka() const;
	int getIkd() const;
	int getIks() const;
	int getUProjModelView() const;
	int getULitColor() const;
	int getULitDirection() const;
	int getULitIntensity() const;
	int getUCameraPosition() const;
};
#endif