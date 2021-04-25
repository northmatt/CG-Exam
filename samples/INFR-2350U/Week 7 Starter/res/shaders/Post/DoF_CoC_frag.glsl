#version 420

layout(location = 0) in vec2 inUV;

out vec4 frag_color;

layout (binding = 0) uniform sampler2D s_depthTex;

uniform float u_focusDist;
uniform float u_focusRange;

float linearize_depth(float d, float zNear, float zFar)
{
	float z_n = 2.0 * d - 1.0;
	return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}

void main() 
{
	float expectedDepth = linearize_depth(texture(s_depthTex, inUV).x, 0.01, 1000.0);
	float coc = clamp((expectedDepth - u_focusDist) / u_focusRange, -1.0, 1.0);
	float isPositive = floor(min(coc,0.0)) + 1.0;
	vec4 source = vec4(abs(coc), coc * isPositive, coc * isPositive, 1.0);

	frag_color.rgb = source.rgb;
	frag_color.a = source.a;
}