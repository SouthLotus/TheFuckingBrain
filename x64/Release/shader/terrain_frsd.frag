#version 330 core

in VERTEX {
	vec3 oVCoord;
	vec3 oVNormal;
} frsd;

struct light {
	vec3 direction;
	vec3 color;
	float intensity;	
};

out vec4 oColor;
uniform light lit;
uniform vec3 cameraPosition;

void main() {
	vec3 nVCoordToCamPos = normalize(cameraPosition - frsd.oVCoord);
	vec3 refLitDirection = reflect(lit.direction, frsd.oVNormal);
	vec3 ambient = vec3(0.3, 0.3, 0.3);
	vec3 diffuse = max(dot(frsd.oVNormal, -lit.direction), 0.0) * lit.intensity * lit.color;
	vec3 specular = pow(max(dot(refLitDirection, nVCoordToCamPos), 0.0), 2) * lit.intensity * lit.color * vec3(0.5, 0.5, 0.5);
	oColor = vec4(ambient + diffuse + specular, 1.0);
}