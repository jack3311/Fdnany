#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 vs_fs_color;

layout (std140) uniform viewInfo
{
	mat4 projection;
	mat4 view;
	mat4 viewProjection;
};

void main(void)
{
	gl_Position = viewProjection * vec4(position, 1.0f);

	vs_fs_color = color;
}