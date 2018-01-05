#version 430 core
layout (location = 0) in vec3 position;

out vec3 outTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(void)
{
	outTexCoord = position;

	gl_Position = projection * view * model * vec4(position, 1.0f);
	//gl_Position = vec4(0.f, 0.f, 0.f, 1.f);
}