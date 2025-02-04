#pragma once
#include "Vertex.h"

#include <d3d12.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

class Mesh
{
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBuffer();
	int GetVertexCount();
	int GetIndexCount();
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
};