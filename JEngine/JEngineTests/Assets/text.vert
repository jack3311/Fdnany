#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 vs_fs_texCoords;

uniform mat4 viewProjectionMatrix;

void main() {
	//Use 1.0 z value to fix depth test
	gl_Position = viewProjectionMatrix * vec4(position, 1.0, 1.0);

	vs_fs_texCoords = texCoords;
}