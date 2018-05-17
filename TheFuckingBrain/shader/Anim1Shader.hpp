#ifndef __anim1_shader
#define __anim1_shader
#include "..\owner\GLProgramOwner.hpp"
class Anim1Shader {
private:
	static const char *VTSD_PATH;
	static const char *FRSD_PATH;
	GLProgramOwner program;
	int iCoord;
	int iNormal;

	int ika;
	int ikd;
	int iks;

	int iBIDs[2];
	int iBWeights[2];
	int vTrans;
	int projViewModel;

	int litColor;
	int litDirection;
	int litIntensity;
	int cameraPosition;
	int worldTrans;
	void init();
	Anim1Shader(bool flag);
public:
	Anim1Shader();
	~Anim1Shader() = default;	
	void useProgram();
	void unUseProgram();
	int getICoord();
	int getINormal();

	int getIka() const;
	int getIkd() const;
	int getIks() const;	

	int getIBIDs(int index);
	int getIBWeights(int index);
	int getVTrans();
	int getProjViewModel();
	int getLitColor() const;
	int getLitDirection() const;
	int getLitIntensity() const;
	int getCameraPosition() const;
	int getWorldTrans() const;
	static Anim1Shader create();
};
#endif