#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl/client.h>


struct Buffer {
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
};