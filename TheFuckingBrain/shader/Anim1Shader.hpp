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
	int iBIDs;
	int iBWeights;
	int vTrans;
	int projViewModel;
	void init();
	Anim1Shader(bool flag);
public:
	Anim1Shader();
	~Anim1Shader() = default;	
	void useProgram();
	void unUseProgram();
	int getICoord();
	int getINormal();
	int getIBIDs();
	int getIBWeights();
	int getVTrans();
	int getProjViewModel();
	static Anim1Shader create();
};
#endif