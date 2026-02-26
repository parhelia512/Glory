#type frag
#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "Internal/Blur.glsl"

layout(location = 0) in vec2 Coord;
#ifdef WITH_ONLY_RED
layout(location = 0) out float out_Color;
#else
layout(location = 0) out vec4 out_Color;
#endif

layout (binding = 0) uniform sampler2D Color;

#define FEATURE_ONLY_RED

#ifdef PUSH_CONSTANTS
layout(push_constant) uniform RenderConstantsUBO
#else
layout(set = 0, std140, binding = 0) readonly uniform RenderConstantsUBO
#endif
{
	uint BlurType;
    int BlurSize;
    float Separation;
    int BinsSize;
} Constants;

void main()
{
    if (Constants.BlurType == BlurType_Box)
        out_Color = BoxBlur(Color, Constants.BlurSize, Constants.Separation);
    else if (Constants.BlurType == BlurType_Median)
	    out_Color = MedianBlur(Color, Constants.BlurSize, Constants.BinsSize);
}
