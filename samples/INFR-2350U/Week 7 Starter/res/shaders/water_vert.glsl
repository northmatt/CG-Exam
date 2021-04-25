#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outColor;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec2 outUV;
layout(location = 4) out vec4 outFragPosLightSpace;

uniform mat4 u_ModelViewProjection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat3 u_NormalMatrix;
uniform vec3 u_LightPos;
uniform mat4 u_LightSpaceMatrix;

uniform float u_Time;

void main() 
{
    vec4 pos = (vec4(inPosition, 1.0));
    pos.z += (
        sin(pos.x * 0.90 + u_Time * 2.5) + sin(pos.x * 0.76 - u_Time * 2.35) + 
        sin(pos.y * 0.86 + u_Time * 2.2) + sin(pos.y * 0.67 - u_Time * 2.14))
        * 0.25;

	gl_Position = u_ModelViewProjection * pos;

	// Lecture 5
	// Pass vertex pos in world space to frag shader
	outPos = ((u_Model * pos)).xyz;

	// Normals
	outNormal = u_NormalMatrix * inNormal;

	// Pass our UV coords to the fragment shader
	outUV = inUV;

	//Pass out the light space fragment pos
	outFragPosLightSpace = u_LightSpaceMatrix * vec4(outPos, 1.0);

	///////////
	outColor = inColor;

}