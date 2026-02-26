#type frag
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 Coord;
layout(location = 0) out float out_Color;
layout (set = 2, binding = 0) uniform sampler2D Normal;
layout (set = 2, binding = 1) uniform sampler2D Depth;
layout (set = 3, binding = 2) uniform sampler2D Noise;

#ifdef PUSH_CONSTANTS
layout(push_constant) uniform RenderConstantsUBO
#else
layout(set = 0, std140, binding = 0) readonly uniform RenderConstantsUBO
#endif
{
	uint CameraIndex;
    int KernelSize;
    float SampleRadius;
    float SampleBias;
    float ResolutionScale;
} Constants;

#include "Internal/Camera.glsl"
#include "Internal/DepthHelpers.glsl"

layout(set = 1, std140, binding = 2) readonly uniform SampleDomeUBO
{
    vec3 SamplePoints[64];
};

void main()
{
    float depth = texture(Depth, Coord).r;
    if (depth == 1.0)
    {
        out_Color = 1.0;
        return;
    }

    CameraData camera = CurrentCamera();
    float ratio = camera.Resolution.x/camera.Resolution.y;
    vec2 screenEdgeDistances = vec2(0.01, 0.01*ratio);

    /* Fade the effect at screen edges to avoid artifacts */
    vec4 screenRange = vec4(smoothstep(0.0, screenEdgeDistances.x, Coord.x),
        1.0-smoothstep(1.0-screenEdgeDistances.x, 1.0, Coord.x),
        smoothstep(0.0, screenEdgeDistances.y, Coord.y),
        1.0-smoothstep(1.0-screenEdgeDistances.y, 1.0, Coord.y)
    );

    float screenFactor = screenRange.x*screenRange.y*screenRange.z*screenRange.w;

    if (screenFactor == 0.0)
    {
        out_Color = 1.0;
        return;
    }

    vec2 noiseScale = vec2(camera.Resolution.x/4.0*Constants.ResolutionScale,
        camera.Resolution.y/4.0*Constants.ResolutionScale);
	vec3 normal = texture(Normal, Coord).xyz*2.0 - 1.0;
    normal = mat3(camera.View)*normalize(normal);
	vec3 randomVec = texture(Noise, Coord*noiseScale).xyz;

	vec4 fragPosition = ViewPosFromDepth(depth, camera.ProjectionInverse, Coord);

    vec3 tangent   = normalize(randomVec - normal*dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN       = mat3(tangent, bitangent, normal);

    float scaledRadius = Constants.SampleRadius*(1.0 - (LinearDepth(depth, camera.zNear, camera.zFar)/camera.zFar));
    scaledRadius = clamp(scaledRadius, 0.1, 1.0);

    float att = 0.0;
    for(int i = 0; i < Constants.KernelSize; ++i)
    {
        // get sample position
        vec3 samplePos = TBN*SamplePoints[i]; // from tangent to view-space
        samplePos = fragPosition.xyz + samplePos*scaledRadius; 

        vec4 offset = vec4(samplePos, 1.0);
        offset      = camera.Projection*offset;    // from view to clip-space
        offset.xyz /= offset.w;               // perspective divide
        offset.xyz  = offset.xyz*0.5 + 0.5; // transform to range 0.0 - 1.0

        float sampleDepth = texture(Depth, offset.xy).r;
        vec3 offsetPosition = ViewPosFromDepth(sampleDepth, camera.ProjectionInverse, Coord).xyz;
        float intensity = smoothstep(0.0, 1.0, scaledRadius/abs(fragPosition.z - offsetPosition.z));
        float occluded = samplePos.z + Constants.SampleBias <= offsetPosition.z ? 1.0 : 0.0;
        occluded *= intensity;

        att += 1.0 / (1.0 + occluded*occluded);
    }
    out_Color = mix(1.0, clamp(att/float(Constants.KernelSize), 0.0, 1.0), screenFactor);
}
