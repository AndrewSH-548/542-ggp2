#include "ShaderIncludes.hlsli"

Texture2D ParticleTexture : register(t0);
SamplerState Sampler : register(s0);

float4 main(VertexToPixel_Particle input) : SV_TARGET
{
    return pow(ParticleTexture.Sample(Sampler, input.uv), 2.2f) * input.colorTint;
}