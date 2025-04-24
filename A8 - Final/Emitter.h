#pragma once
#define MAX_PARTICLE_COUNT 40
#define RandomRange(min, max) ((float)rand() / RAND_MAX * (max - min) + min)

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#include "SimpleShader.h"
#include "Camera.h"
#include "Material.h"
#include "Transform.h"

using namespace std;
using namespace DirectX;

struct Particle {
	float emitTime;
	XMFLOAT3 startPosition;

	float orientation;
	
	XMFLOAT4 color;
};

class Emitter
{
public:
	// Methods
	Emitter(XMFLOAT3 position, float rotation, float scale, float lifetime, int pps, float dispersalRange, const wchar_t* texturePath, shared_ptr<Material> material, bool randomColor = false);
	~Emitter() {
		delete[] particles;
	};
	void Update(float delta, float currentTime);
	void Draw(Camera camera, float currentTime);
private:
	// Particle Data
	Particle* particles;
	int currentParticleCount;
	shared_ptr<Material> particleMaterial;
	void CreateParticle(int index, float currentTime);
	void UpdateExistingParticles(float currentTime);
	void LoopParticleIndices();

	// Emission Properties
	// EXTRA: Uses transform instead of just position
	shared_ptr<Transform> transform;
	int firstDeadParticle;
	int firstLivingParticle;
	float particleLifetime;
	int particlesPerSecond;
	float secondsBetweenParticles;
	float timeSinceLastEmission;
	
	// EXTRA: adjust range of positions for particles to spawn randomly
	float dispersalRange;

	// EXTRA: enable this to randomly color a particle upon spawning
	bool randomColor;

	// EXTRA: set negative rotation float to enable random rotation upon spawning
	bool randomRotation;

	// Rendering Resources
	Microsoft::WRL::ComPtr<ID3D11Buffer> particleDataBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> particleDataSRV;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> particleTexture;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	void CreateStructuredBuffers();
	void CopyAllBufferData(XMFLOAT4X4 view, XMFLOAT4X4 projection, float currentTime);

	shared_ptr<SimplePixelShader> pixelShader;
	shared_ptr<SimpleVertexShader> vertexShader;

	inline XMFLOAT3 RandomPosition(float range) {
		return XMFLOAT3(
			transform->GetPosition().x + RandomRange(-range, range),
			transform->GetPosition().y + RandomRange(-range, range),
			transform->GetPosition().z + RandomRange(-range, range));
	}
	
	inline XMFLOAT4 RandomColor() {
		return XMFLOAT4(RandomRange(0, 1), RandomRange(0, 1), RandomRange(0, 1), 1);
	}

	inline float RandomRotation() {
		return RandomRange(0, XM_PI * 2);
	}
};

