#include "ShaderIncludes.hlsli"

Texture2D ParticleTexture;
SamplerState Sampler;

float4 main(VertexToPixel_Particle input) : SV_TARGET
{
    return ParticleTexture.Sample(Sampler, input.uv) * input.colorTint;
}