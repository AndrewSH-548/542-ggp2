#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl/client.h>

using namespace DirectX;

struct BufferVertex {
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;
};

struct BufferPixel {
	XMFLOAT2 uvScale;
	XMFLOAT2 uvOffset;
	XMFLOAT3 cameraPos;
	int lightCount;
	Light lights[2];
};