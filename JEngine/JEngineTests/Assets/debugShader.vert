#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 vs_fs_color;

uniform mat4 viewProjectionMatrix;

void main(void)
{
	gl_Position = viewProjectionMatrix * vec4(position, 1.0f);

	vs_fs_color = color;
}