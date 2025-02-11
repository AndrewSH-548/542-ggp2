#include "ShaderIncludes.hlsli"

Texture2D Albedo : register(t0);
Texture2D Normal : register(t1);
Texture2D Roughness : register(t2);
Texture2D Metalness : register(t3);
SamplerState Sampler : register(s0);

cbuffer ExternalData : register(b0) {
	float2 uvScale;
	float2 uvOffset;
	float3 cameraPosition;
	int lightCount;
	Light lights[2];
}

// LIGHT CONSTRUCTION FUNCTIONS =============

float Attenuate(Light light, float3 worldPosition)
{
    float dist = distance(light.position, worldPosition);
    float attenuation = saturate(1.0f - (dist * dist / (light.range * light.range)));
    return attenuation * attenuation;
}

float3 constructLight(VertexToPixel input, Light light, float3 surfaceColor, float3 specularColor, float roughness, float metalness)
{
    float3 diffuseTerm = DiffusePBR(input.normal, normalize(light.direction));
    float3 fresnel;
    float specular = MicrofacetBRDF(input.normal, normalize(light.direction), input.worldPosition - cameraPosition, roughness, specularColor, fresnel);

    float3 balancedDiffuse = DiffuseEnergyConserve(diffuseTerm, fresnel, metalness);

    float3 finalLight = (balancedDiffuse * surfaceColor + specular) * light.intensity * light.color;
    if (light.type == LIGHT_TYPE_POINT)
        finalLight *= Attenuate(light, input.worldPosition);
    return finalLight;
}

float3 transformNormal(float3 normal, float3 tangent, float3 unpackedNormal)
{
    float3 gsTangent = normalize(tangent - normal * dot(tangent, normal));
    float3 biTangent = cross(gsTangent, normal);
    float3x3 TBN = float3x3(tangent, biTangent, normal);
    return mul(unpackedNormal, TBN);
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent);

    float3 unpackedNormal = normalize(Normal.Sample(Sampler, input.uv).rgb * 2 - 1);
    input.normal = transformNormal(input.normal, input.tangent, unpackedNormal);

    float3 finalLight;

    float3 surfaceColor = pow(Albedo.Sample(Sampler, input.uv).rgb, 2.2f);
    float roughness = Roughness.Sample(Sampler, input.uv).r;
    float metalness = Metalness.Sample(Sampler, input.uv).r;
    float3 specularColor = lerp(F0_NON_METAL, surfaceColor, metalness);

    for (int i = 0; i < 2; i++)
    {
        if (lights[i].type == LIGHT_TYPE_POINT)
        {
            Light editableLight = lights[i];
            editableLight.direction = input.worldPosition - editableLight.position;
            finalLight += constructLight(input, editableLight, surfaceColor, specularColor, roughness, metalness);
        }
        else
        {
            float3 lightResult = constructLight(input, lights[i], surfaceColor, specularColor, roughness, metalness);
            finalLight += lightResult;
        }
    }


	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
    return float4(pow(finalLight, 1.0f / 2.2f), 1);
}