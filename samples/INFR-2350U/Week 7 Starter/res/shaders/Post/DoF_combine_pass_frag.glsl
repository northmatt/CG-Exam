#version 420

layout(location = 0) in vec2 inUV;

layout (binding = 0) uniform sampler2D s_screenTex;
layout (binding = 1) uniform sampler2D s_blurTex;
layout (binding = 2) uniform sampler2D s_CoC;

out vec4 frag_colour;

void main() {
	frag_colour = vec4(mix(
						texture(s_screenTex, inUV).rgb,
						texture(s_blurTex, inUV).rgb,
						texture(s_CoC, inUV).r),
					1.0);
}