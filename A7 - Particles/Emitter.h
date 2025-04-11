#pragma once
#define MAX_PARTICLE_COUNT 40

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#include "SimpleShader.h"
#include "Camera.h"

using namespace DirectX;

struct Particle {
	float emitTime;
	XMFLOAT3 startPosition;
};

class Emitter
{
public:
	// Methods
	Emitter(XMFLOAT3 pos, float lifetime, int pps, XMFLOAT4 colorTint, const wchar_t* texturePath, const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath);
	~Emitter() {
		delete[] particles;
	};
	void Update(float delta, float currentTime);
	void Draw(Camera camera, float currentTime);
private:
	// Particle Data
	Particle* particles;
	int currentParticleCount;
	XMFLOAT4 particleColor;
	void CreateParticle(int index, float currentTime);
	void UpdateExistingParticles(float currentTime);
	void LoopParticleIndices();

	// Emission Properties
	XMFLOAT3 position;
	int firstDeadParticle;
	int firstLivingParticle;
	float particleLifetime;
	int particlesPerSecond;
	float secondsBetweenParticles;
	float timeSinceLastEmission;

	// Rendering Resources
	Microsoft::WRL::ComPtr<ID3D11Buffer> particleDataBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> particleDataSRV;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRV;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	void CreateStructuredBuffers();
	void CopyAllBufferData(XMFLOAT4X4 view, XMFLOAT4X4 projection, float currentTime);

	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;
};

