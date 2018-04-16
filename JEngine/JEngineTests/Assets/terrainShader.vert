#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec4 texFactors;

out vec3 vs_fs_position;
out vec3 vs_fs_normal;
out vec2 vs_fs_texCoords;
out vec4 vs_fs_texFactors;


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
	
	vs_fs_normal = normal;
	vs_fs_texCoords = texCoords;
	vs_fs_texFactors = texFactors;

	vs_fs_position = position;
}