#version 420

// come from the vertex shader
layout(location = 0) in vec3 in_Norm;
layout(location = 1) in vec3 in_Light;
layout(location = 2) in vec3 in_Half;
layout(location = 3) in float in_LightDistFromPos;
layout(location = 4) in float in_Height;

out vec4 FragCol;

void main(void)
{
	vec3 normal = normalize(in_Norm);
	vec3 light = normalize(in_Light);
	vec3 h = normalize(in_Half);

    vec4 ambient  = vec4(0.0, 0.65, 0.75, 1.0);
    vec4 diffuse  = vec4(0.5, 0.65, 0.75, 1.0);
    vec4 specular = vec4(1.0, 0.25, 0.0,  1.0);
 
    float ambientScale  = 0.30;
    float diffuseScale  = 0.30;
    float specularScale = 1.80;

	float d = dot(normal, light);
    bool facing = d > 0.0;

    FragCol = ambient  * ambientScale +
				diffuse  * diffuseScale * max(d, 0);

	if(facing) 
		FragCol += specular * specularScale * max(pow(dot(normal, h), 120.0), 0.0);

	FragCol += vec4(0,107.0/255.0,82.0/255.0,1) * max( 0, mix(0.0,0.03,in_Height));

}