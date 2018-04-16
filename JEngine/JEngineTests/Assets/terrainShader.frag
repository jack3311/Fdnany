#version 430 core

out vec4 color;

in vec3 vs_fs_position;
in vec3 vs_fs_normal;
in vec2 vs_fs_texCoords;
in vec4 vs_fs_texFactors;


uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

void main()
{
	vec3 textureSampleSum = vec3(0, 0, 0);
	textureSampleSum += vs_fs_texFactors.r * texture(texture0, vs_fs_texCoords).rgb;
	textureSampleSum += vs_fs_texFactors.g * texture(texture1, vs_fs_texCoords).rgb;
	textureSampleSum += vs_fs_texFactors.b * texture(texture2, vs_fs_texCoords).rgb;
	textureSampleSum += vs_fs_texFactors.a * texture(texture3, vs_fs_texCoords).rgb;

	color = vec4(textureSampleSum, 1);
	//color = vec4(vs_fs_position.y, 1, 1, 1);
	//color = 
}