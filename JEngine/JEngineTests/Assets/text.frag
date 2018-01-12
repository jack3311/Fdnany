#version 330 core

in vec2 vs_fs_texCoords;

out vec4 color;

uniform sampler2D texture0;

//uniform vec3 textColor;

void main(){
	//vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture0, vs_fs_texCoords).r);
	//color = sampled;

	color = vec4(0.f, 0.f, 0.f, texture(texture0, vs_fs_texCoords).r);

	//color = vec4(0.f, 0.f, 1.f, 1.f);
	//color = vec4(vs_fs_texCoords, 0.f, 1.f);

	//color = vec4(textColor, 1.0) * sampled;
}