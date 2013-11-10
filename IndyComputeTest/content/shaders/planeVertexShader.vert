#version 420

in vec2 Position;

out vec2 TexCoord;

void main(void)
{
	TexCoord = Position.xy * 0.5f + 0.5f;

	gl_Position =  vec4( Position.x, Position.y, 0, 1.0);
}