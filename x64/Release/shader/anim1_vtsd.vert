#version 330 core

const int BLIMIT = 4;
const int MLIMIT = 10;

in vec4 iCoord;
in vec3 iNormal;
in int iBIDs[BLIMIT];
in float iBWeights[BLIMIT];
uniform mat4 vTrans[MLIMIT];
uniform mat4 projViewModel;

void main() {
	float totalWeight = iBWeights[0] + iBWeights[1] + 
	iBWeights[2] + iBWeights[3];
	if(totalWeight <= 0.0)
		gl_Position = projViewModel * iCoord;
	else {
		mat4 trans = vTrans[iBIDs[0]] * iBWeights[0] +
		vTrans[iBIDs[1]] * iBWeights[1] +
		vTrans[iBIDs[2]] * iBWeights[2] +
		vTrans[iBIDs[3]] * iBWeights[3];
		gl_Position = projViewModel * trans * iCoord;
	}
}