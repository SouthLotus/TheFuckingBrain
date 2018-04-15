#version 330 core

in vec3 oVNormal;
in vec2 oVTexCoord;

out vec4 oColor;
uniform sampler2D sampler;

void main() {
	oColor = texture(sampler, oVTexCoord);
}