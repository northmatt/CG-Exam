#version 420

layout(location = 0) in vec2 inUV;

out vec4 frag_color;

layout (binding = 0) uniform sampler2D s_screenTex;

const float weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

uniform float u_TexelSize;
uniform float u_Spread;

vec4 GetColour(int offset) {
	vec4 texCol = texture(s_screenTex, vec2(inUV.x + u_TexelSize * u_Spread * offset, inUV.y));
	texCol.rgb *= weights[abs(offset)];
	
	return vec4(texCol.rgb, 1.0);
}

//done with the guidance tut bloom code and GDW DoF blur frag shader
void main() 
{
	vec4 result = texture(s_screenTex, inUV) * weights[0];
	
	result += GetColour(-4);
	result += GetColour(-3);
	result += GetColour(-2);
	result += GetColour(-1);
	result += GetColour(1);
	result += GetColour(2);
	result += GetColour(3);
	result += GetColour(4);

	frag_color = vec4(result.rgb, 1.0);
}