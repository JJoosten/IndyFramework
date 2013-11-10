#version 420

in vec2 TexCoord;

uniform sampler2D Texture;

out vec4 FragmentColor;

void main(void)
{
	const vec4 texel = texture2D( Texture, TexCoord);
	
	FragmentColor = texel;
}