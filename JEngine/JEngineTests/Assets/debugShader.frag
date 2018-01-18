#version 430 core

out vec4 color;

in vec3 vs_fs_color;

uniform sampler2D texture0;

void main()
{
	color = vec4(vs_fs_color, 1.f);
}