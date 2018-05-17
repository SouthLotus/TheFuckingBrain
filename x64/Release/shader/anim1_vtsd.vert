#version 330 core

in vec4 iCoord;
in vec3 iNormal;
in ivec3 iBIDs[2];
in vec3 iBWeights[2];

in vec3 ika;
in vec3 ikd;
in vec3 iks;

out VERTEX {
	vec3 oVCoord;
	vec3 oVNormal;
	vec3 oka;
	vec3 okd;
	vec3 oks;
} vtsd;

uniform mat4 vTrans[] = mat4[10](
	mat4(1.0), mat4(1.0), mat4(1.0), 
	mat4(1.0), mat4(1.0), mat4(1.0),
	mat4(1.0), mat4(1.0), mat4(1.0), mat4(1.0));
uniform mat4 projViewModel;
uniform mat4 worldTrans;

void main() {
	vtsd.oka = ika;
	vtsd.okd = ikd;
	vtsd.oks = iks;
	float totalWeight = iBWeights[0][0] + 
	iBWeights[0][1] +
	iBWeights[0][2] + 
	iBWeights[1][0] +
	iBWeights[1][1] +
	iBWeights[1][2];
	if(totalWeight <= 0.1) {
		vtsd.oVCoord = vec3(iCoord);
		vtsd.oVNormal = iNormal;
		gl_Position = projViewModel * iCoord;
	}
	else {
		mat4 trans = vTrans[iBIDs[0][0]] * iBWeights[0][0] +
		vTrans[iBIDs[0][1]] * iBWeights[0][1] +
		vTrans[iBIDs[0][2]] * iBWeights[0][2] +
		vTrans[iBIDs[1][0]] * iBWeights[1][0] +
		vTrans[iBIDs[1][1]] * iBWeights[1][1] +
		vTrans[iBIDs[1][2]] * iBWeights[1][2];
		vec4 newCoord = trans * iCoord;
		trans[3][0] = 0;
		trans[3][1] = 0;
		trans[3][2] = 0;
		vtsd.oVCoord = vec3(newCoord);
		vtsd.oVNormal = vec3(trans * vec4(iNormal, 1.0));
		gl_Position = projViewModel * worldTrans * newCoord;
	}
}