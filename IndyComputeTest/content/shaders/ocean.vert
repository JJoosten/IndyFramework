#version 420

uniform mat4 ViewMat;
uniform mat4 ProjMat;
uniform mat4 NormMat;

layout(location = 0) in vec3 in_Pos;
layout(location = 1) in vec3 in_Norm;

layout(location = 0) out vec4 out_PosViewSpace;
layout(location = 1) out vec3 out_Norm;

void main(void)
{
	vec4 posCached = vec4(in_Pos, 1);

	gl_Position = ProjMat * ViewMat * posCached;

	out_PosViewSpace = (ViewMat * posCached);
	out_Norm = (NormMat * vec4( in_Norm, 0)).xyz;

}

