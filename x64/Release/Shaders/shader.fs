// Fragment shader code

#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

// a texture
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixLevel;

void main()
{
	FragColor = mix(texture(texture1, TexCoord), 
		texture(texture2, vec2(TexCoord*10)), mixLevel);
}