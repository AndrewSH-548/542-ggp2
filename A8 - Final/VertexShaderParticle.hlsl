#include "ShaderIncludes.hlsli"

// MAKE SURE THIS MATCHES THE CONST IN EMITTER.H
#define MAX_PARTICLE_COUNT 40

struct Particle
{
    float emitTime;
    float3 startPosition;
    
    float orientation;
    
    float4 color;
};

cbuffer ExternalData : register(b0)
{
    matrix view;
    matrix projection;
    float particleLifetime;
    float currentTime;
    
    float3 baseOrientation;
    float scale;
}

StructuredBuffer<Particle> particleData : register(t0);

VertexToPixel_Particle main(uint id : SV_VertexID)
{
    VertexToPixel_Particle output;
    
    uint particleID = id / 4;
    uint cornerID = id % 4;
    
    Particle p = particleData.Load(particleID);
    float age = currentTime - p.emitTime;
    float3 position = p.startPosition;
    
    // Create offset array 
    float2 offsets[4];
    offsets[0] = float2(-scale, scale);         //Upper Left
    offsets[1] = float2(scale, scale); //Upper Right
    offsets[2] = float2(scale, -scale); //Bottom Right
    offsets[3] = float2(-scale, -scale); //Bottom Left
    
    // Rotate the offset with a matrix
    float s, c, rotation = baseOrientation + float3(p.orientation,
    p.orientation, p.orientation);
    sincos(rotation, s, c);
    float2x2 rotationMatrix =
    {
        c, s,
        -s, c
    };
    
    float2 rotatedOffset = mul(offsets[cornerID], rotationMatrix);
    
    // Enact billboarding to move corners
    // EXTRA: Particle shrinks until its death.
    position += lerp(float3(view._11, view._12, view._13) * rotatedOffset.x, 0, age / particleLifetime);
    position += lerp(float3(view._21, view._22, view._23) * rotatedOffset.y, 0, age / particleLifetime);
    
    matrix viewProjection = mul(projection, view);
    output.position = mul(viewProjection, float4(position, 1));
    
    float2 uvs[4];
    uvs[0] = float2(-1, 1); //Upper Left
    uvs[1] = float2(1, 1); //Upper Right
    uvs[2] = float2(1, -1); //Bottom Right
    uvs[3] = float2(-1, -1); //Bottom Left
    
    output.uv = saturate(uvs[cornerID]);
    output.colorTint = p.color;
    return output;
}