#pragma once
#include "Vertex.h"

#include <d3d12.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

struct MeshRaytracingData
{
	D3D12_GPU_DESCRIPTOR_HANDLE IndexBufferSRV{ };
	D3D12_GPU_DESCRIPTOR_HANDLE VertexBufferSRV{ };
	Microsoft::WRL::ComPtr<ID3D12Resource> BLAS;
	unsigned int HitGroupIndex = 0;
};

class Mesh
{
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBuffer() { return vertexBuffer; }
	Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBuffer() { return indexBuffer; }
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() { return vbView; }
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() { return ibView; }
	int GetVertexCount() { return vertexCount; }
	int GetIndexCount() { return indexCount; }
	MeshRaytracingData GetRaytracingData() { return raytracingData; }

	void Draw();
	Mesh(Vertex vertices[], int vertexCount, unsigned int indices[], int indexCount);
	Mesh(const wchar_t* filePath);
	~Mesh();

private:
	void ConstructBuffers(Vertex* vertices, int vertexCount, unsigned int* indices, int indexCount);
	void CalculateTangents(Vertex* vertices, int vertexCount, unsigned int* indices, int indexCount);

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	D3D12_INDEX_BUFFER_VIEW ibView{};
	int vertexCount;
	int indexCount;
	MeshRaytracingData raytracingData;
};

