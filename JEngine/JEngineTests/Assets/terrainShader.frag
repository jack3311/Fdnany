#version 430 core

out vec4 color;

in vec3 vs_fs_normal;
in vec2 vs_fs_texCoords;
in vec3 vs_fs_position;

uniform sampler2D texture0;

void main()
{
	//color = vec4(texture(texture0, vs_fs_texCoords).rgba);
	color = vec4(vs_fs_position.y, 1, 1, 1);
}