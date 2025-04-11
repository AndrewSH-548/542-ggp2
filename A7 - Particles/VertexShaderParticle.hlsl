#include "ShaderIncludes.hlsli"

// MAKE SURE THIS MATCHES THE CONST IN EMITTER.H
#define MAX_PARTICLE_COUNT 40

struct Particle
{
    float emitTime;
    float3 startPosition;
};


cbuffer ExternalData : register(b0)
{
    matrix view;
    matrix projection;
    float3 colorTint;
    float currentTime;
}

StructuredBuffer<Particle> particleData : register(t0);

VertexToPixel_Particle main(uint id : SV_VertexID)
{
    VertexToPixel_Particle output;
    
    uint particleID = id / 4;
    uint cornerID = id % 4;
    
    Particle p = particleData.Load(id);
    float age = currentTime - p.emitTime;
    float3 position = p.startPosition;
    
    // Create offset array 
    float2 offsets[4];
    offsets[0] = float2(-1, 1);         //Upper Left
    offsets[1] = float2(1, 1);          //Upper Right
    offsets[2] = float2(1, -1);         //Bottom Right
    offsets[3] = float2(-1, -1);        //Bottom Left
    
    // Enact billboarding to move corners
    position += float3(view._11, view._12, view._13) * offsets[cornerID].x * 10;
    position += float3(view._21, view._22, view._23) * offsets[cornerID].y * 10;
    
    matrix viewProjection = mul(projection, view);
    output.position = mul(viewProjection, float4(position, 1));
    
    float2 uvs[4];
    uvs[0] = float2(-1, 1); //Upper Left
    uvs[1] = float2(1, 1); //Upper Right
    uvs[2] = float2(1, -1); //Bottom Right
    uvs[3] = float2(-1, -1); //Bottom Left
    
    output.uv = uvs[cornerID];
    output.colorTint = float4(colorTint, 1);
    return output;
}