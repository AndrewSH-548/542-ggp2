#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl/client.h>

using namespace DirectX;

class Material
{
public:
	Material(Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState, XMFLOAT3 colorTint, XMFLOAT2 uvScale, XMFLOAT2 uvOffset, float roughness, float metal);
	void AddTexture(D3D12_CPU_DESCRIPTOR_HANDLE srv, int slot);
	void FinalizeMaterial();

	XMFLOAT3 GetColorTint() { return colorTint; }
	XMFLOAT2 GetUVScale() { return uvScale; }
	XMFLOAT2 GetUVOffset() { return uvOffset; }
	float GetRoughness() { return roughness; }
	float GetMetal() { return metal; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetFinalGPUHandleForSRVs() { return finalGPUHandleForSRVs; }
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState() { return pipelineState; }

private:
	XMFLOAT3 colorTint;
	XMFLOAT2 uvScale;
	XMFLOAT2 uvOffset;
	float roughness;
	float metal;

	bool finalized;
	int highestSRVSlot;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSRVsBySlot[128];
	D3D12_GPU_DESCRIPTOR_HANDLE finalGPUHandleForSRVs;
};
