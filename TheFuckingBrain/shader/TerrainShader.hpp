#ifndef __terrain_shader
#define __terrain_shader
#include "..\owner\GLProgramOwner.hpp"
class TerrainShader {
private:
	const static char *VTSD_PATH;
	const static char *FRSD_PATH;
	GLProgramOwner programOwner;
	int iVCoord;
	int iVNormal;
	int uProjModelView;
	int uLitColor;
	int uLitDirection;
	int uLitIntensity;
	int uCameraPosition;
public:
	TerrainShader() = default;
	~TerrainShader() = default;
	void init();
	void useProgram();
	void unUseProgram();
	int getIVCoord() const;
	int getIVNormal() const;
	int getUProjModelView() const;
	int getULitColor() const;
	int getULitDirection() const;
	int getULitIntensity() const;
	int getUCameraPosition() const;
};
#endif