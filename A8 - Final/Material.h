#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl/client.h>

using namespace DirectX;

class Material
{
public:
	Material(XMFLOAT3 colorTint, XMFLOAT2 uvScale, XMFLOAT2 uvOffset, Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState);
	void AddTexture(D3D12_CPU_DESCRIPTOR_HANDLE srv, int slot);
	void FinalizeMaterial();

	XMFLOAT2 GetUVScale();
	XMFLOAT2 GetUVOffset();
	D3D12_GPU_DESCRIPTOR_HANDLE GetFinalGPUHandleForSRVs();
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState();

private:
	XMFLOAT3 colorTint;
	XMFLOAT2 uvScale;
	XMFLOAT2 uvOffset;
	bool finalized;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSRVsBySlot[4];
	D3D12_GPU_DESCRIPTOR_HANDLE finalGPUHandleForSRVs;
};
