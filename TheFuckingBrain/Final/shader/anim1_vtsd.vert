#version 330 core

in vec4 iCoord;
in vec3 iNormal;
in ivec3 iBIDs[2];
in vec3 iBWeights[2];
uniform mat4 vTrans[] = mat4[10](
	mat4(1.0), mat4(1.0), mat4(1.0), 
	mat4(1.0), mat4(1.0), mat4(1.0),
	mat4(1.0), mat4(1.0), mat4(1.0), mat4(1.0));
uniform mat4 projViewModel;
void main() {
	float totalWeight = iBWeights[0][0] + 
	iBWeights[0][1] +
	iBWeights[0][2] + 
	iBWeights[1][0] +
	iBWeights[1][1] +
	iBWeights[1][2];
	if(totalWeight <= 0.1) {
		gl_Position = projViewModel * iCoord;
	}
	else {
		mat4 trans = vTrans[iBIDs[0][0]] * iBWeights[0][0] +
		vTrans[iBIDs[0][1]] * iBWeights[0][1] +
		vTrans[iBIDs[0][2]] * iBWeights[0][2] +
		vTrans[iBIDs[1][0]] * iBWeights[1][0] +
		vTrans[iBIDs[1][1]] * iBWeights[1][1] +
		vTrans[iBIDs[1][2]] * iBWeights[1][2];
		gl_Position = projViewModel * trans * iCoord;
	}
}