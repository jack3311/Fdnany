#version 430 core

out vec4 color;

in vec2 vs_fs_texCoords;

uniform sampler2D texture0;

void main()
{
	color = vec4(texture(texture0, vs_fs_texCoords).rgba);
}