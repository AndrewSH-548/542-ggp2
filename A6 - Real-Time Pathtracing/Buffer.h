#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#include "Light.h"

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

struct RaytracingSceneData {
	XMFLOAT4X4 inverseViewProjection;
	XMFLOAT3 cameraPosition;
	int raysPerPixel;
};

struct Texture {

};

#define MAX_INSTANCES_PER_BLAS 100
struct RaytracingEntityData {
	XMFLOAT4 color[MAX_INSTANCES_PER_BLAS];
};