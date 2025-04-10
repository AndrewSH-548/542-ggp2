#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

using namespace DirectX;

struct Particle {
	float emitTime;
	XMFLOAT3 startPosition;
};

class Emitter
{
public:
	// Methods
	Emitter(XMFLOAT3 pos, int max, float lifetime, int pps) : 
		position(pos),
		maxParticleCount(max), 
		particleLifetime(lifetime), 
		particlesPerSecond(pps) 
	{
		secondsBetweenParticles = 1.0f / pps;
		currentParticleCount = 0;
		firstDeadParticle = 0;
		firstLivingParticle = 0;
	}

	void Update(float delta, float currentTime);
	void Draw();

	void CreateParticle(int index, float currentTime);
	void UpdateExistingParticles(float currentTime);
	void LoopParticleIndices();
private:
	// Particle Data
	Particle* particles;
	int currentParticleCount;
	int maxParticleCount;

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
};

