#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 vs_fs_texCoords;

layout (std140) uniform viewInfo
{
	mat4 projection;
	mat4 view;
	mat4 viewProjection;
};

uniform mat4 modelViewProjection;

void main(void)
{
	gl_Position = modelViewProjection * vec4(position, 1.0f);

	vs_fs_texCoords = texCoords;
}