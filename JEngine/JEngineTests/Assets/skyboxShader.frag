#version 430 core

in vec3 outTexCoord;

out vec4 color;

uniform samplerCube skybox;

void main()
{
	vec4 texColor = texture(skybox, outTexCoord);

	color = texColor;
	//color = vec4(1.f, 0.f, 0.f, 1.f);
}