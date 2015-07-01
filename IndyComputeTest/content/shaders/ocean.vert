#version 420

uniform mat4 ViewMat;
uniform mat4 ProjMat;
uniform mat4 NormMat;

layout(location = 0) in vec4 in_Pos;
layout(location = 4) in vec4 in_Norm;

layout(location = 0) out vec3 out_Norm;
layout(location = 1) out vec3 out_Light;
layout(location = 2) out vec3 out_Half;
layout(location = 3) out float out_LightDistFromPos;
layout(location = 4) out float out_Height;

void main(void)
{
	vec3 posInViewSpace = (ViewMat * vec4(in_Pos.xyz, 1)).xyz;

	gl_Position = ProjMat * ViewMat * vec4(in_Pos.xyz, 1);

	out_Height = in_Pos.y;

	vec3 lightVec = (ViewMat * vec4( vec3(0,1,0), 1)).xyz - posInViewSpace;

	out_LightDistFromPos = length(lightVec);

	out_Light = normalize(lightVec);
	
	out_Norm = (inverse(transpose(ViewMat)) * vec4( in_Norm.xyz, 0)).xyz;

	out_Half = out_Light + normalize(-posInViewSpace.xyz);
}

