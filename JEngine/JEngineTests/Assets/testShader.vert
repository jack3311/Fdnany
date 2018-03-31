#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 vs_fs_texCoords;

uniform mat4 modelViewProjectionMatrix;

void main(void)
{
	gl_Position = modelViewProjectionMatrix * vec4(position, 1.0f);

	vs_fs_texCoords = texCoords;
}