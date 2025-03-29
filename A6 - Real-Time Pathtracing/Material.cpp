#include "Material.h"
#include "Graphics.h"

Material::Material(Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState, XMFLOAT3 colorTint, XMFLOAT2 uvScale, XMFLOAT2 uvOffset, float roughness, float metal = 0.0f) :
	pipelineState(pipelineState), colorTint(colorTint), uvScale(uvScale), uvOffset(uvOffset), roughness(roughness), metal(metal)
{
	finalized = false;
	finalGPUHandleForSRVs = {};
}

void Material::AddTexture(D3D12_CPU_DESCRIPTOR_HANDLE srv, int slot) {
	if (finalized || slot < 0 || slot >= 128) return;
	
	// Order:
	// 1 - Albedo
	// 2 - Normal
	// 3 - Roughness
	// 4 - Metalness
	textureSRVsBySlot[slot] = srv;
	highestSRVSlot = max(highestSRVSlot, slot);
}

void Material::FinalizeMaterial() {
	if (finalized) return;

	for (int i = 0; i < 4; i++) {
		if (i == 0)
			finalGPUHandleForSRVs = Graphics::CopySRVsToDescriptorHeapAndGetGPUDescriptorHandle(textureSRVsBySlot[i], 1);
		else 
			Graphics::CopySRVsToDescriptorHeapAndGetGPUDescriptorHandle(textureSRVsBySlot[i], 1);
	}

	finalized = true;
}