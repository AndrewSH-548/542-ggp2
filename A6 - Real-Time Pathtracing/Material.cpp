#include "Material.h"
#include "Graphics.h"

Material::Material(XMFLOAT3 colorTint, XMFLOAT2 uvScale, XMFLOAT2 uvOffset, float roughness, bool isReflective, Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState) {
	this->colorTint = colorTint;
	this->uvScale = uvScale;
	this->uvOffset = uvOffset;
	this->pipelineState = pipelineState;
	this->roughness = roughness;
	this->isReflective = isReflective;
	finalGPUHandleForSRVs = {};
	//ZeroMemory(textureSRVsBySlot, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * 128);
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