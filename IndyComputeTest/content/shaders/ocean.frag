#version 420

uniform vec3 SunLocationWorldSpace = vec3(1,1,0);

// come from the vertex shader
layout(location = 0) in vec3 in_PosViewSpace;
layout(location = 1) in vec3 in_Norm;

out vec4 FragCol;

void main(void)
{
	vec3 normNormalized = normalize( in_Norm);



	FragCol = vec4(1.0,1.0,1.0,1.0);
}