#include <iostream>

#include "Emitter.h"
#include "Graphics.h"
#include "WICTextureLoader.h"

using namespace std;

Emitter::Emitter(XMFLOAT3 pos, float lifetime, int pps, XMFLOAT4 colorTint, const wchar_t* texturePath, const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath) :
	position(pos),
	particleLifetime(lifetime),
	particlesPerSecond(pps),
	particleColor(colorTint)
{
	secondsBetweenParticles = 1.0f / pps;
	currentParticleCount = 0;
	firstDeadParticle = 0;
	firstLivingParticle = 0;
	timeSinceLastEmission = 0;
	particles = new Particle[MAX_PARTICLE_COUNT];
	CreateStructuredBuffers();
	CreateWICTextureFromFile(Graphics::Device.Get(), Graphics::Context.Get(), texturePath, 0, &textureSRV);
	vertexShader = make_shared<SimpleVertexShader>(Graphics::Device, Graphics::Context, vertexShaderPath);
	pixelShader = make_shared<SimplePixelShader>(Graphics::Device, Graphics::Context, pixelShaderPath);
}

void Emitter::CreateStructuredBuffers() {
	// Make a dynamic buffer to hold all particle data on GPU
	// Note: We'll be overwriting this every frame with new lifetime data
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = sizeof(Particle);
	desc.ByteWidth = sizeof(Particle) * MAX_PARTICLE_COUNT;
	Graphics::Device->CreateBuffer(&desc, 0, particleDataBuffer.GetAddressOf());
	
	// Create an SRV that points to a structured buffer of particles
	// so we can grab this data in a vertex shader
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = MAX_PARTICLE_COUNT;
	Graphics::Device->CreateShaderResourceView(particleDataBuffer.Get(), &srvDesc, particleDataSRV.GetAddressOf());

	// Create index buffer
	// Start with the indices. These will always be for a quad, so no customization is required.
	UINT* indices = new UINT[MAX_PARTICLE_COUNT * 6];
	int indexCount = 0;
	for (int i = 0; i < MAX_PARTICLE_COUNT * 4; i += 4) {
		indices[indexCount++] = i;
		indices[indexCount++] = i + 1;
		indices[indexCount++] = i + 2;
		indices[indexCount++] = i;
		indices[indexCount++] = i + 2;
		indices[indexCount++] = i + 3;
	}

	// Write the buffer desc
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;	// Will NEVER change
	ibd.ByteWidth = sizeof(unsigned int) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;	// Tells Direct3D this is an index buffer
	ibd.CPUAccessFlags = 0;	// Note: We cannot access the data from C++ (this is good)
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Specify the initial data for this buffer, similar to above
	D3D11_SUBRESOURCE_DATA initialIndexData = {};
	initialIndexData.pSysMem = indices; // pSysMem = Pointer to System Memory

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	Graphics::Device->CreateBuffer(&ibd, &initialIndexData, indexBuffer.GetAddressOf());

	// Create sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;								//Lerp
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	Graphics::Device->CreateSamplerState(&samplerDesc, &samplerState);
}

void Emitter::Update(float delta, float currentTime) {
	// Progress in time
	timeSinceLastEmission += delta;
	printf("time: %f first", timeSinceLastEmission);
	UpdateExistingParticles(currentTime);

	// Emit a particle 
	if (timeSinceLastEmission >= secondsBetweenParticles) {
		if (currentParticleCount < MAX_PARTICLE_COUNT) {
			CreateParticle(firstLivingParticle + currentParticleCount, currentTime);

			// Add a space to the chunk of alive particles
			currentParticleCount++;
			firstDeadParticle++;
			LoopParticleIndices();
		}

		timeSinceLastEmission = 0;
	}
}

void Emitter::Draw(Camera camera, float currentTime) {
	CopyAllBufferData(camera.GetViewMatrix(), camera.GetProjectionMatrix(), currentTime);
	Graphics::Context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Graphics::Context->DrawIndexed(currentParticleCount * 6, 0, 0);
}

void Emitter::CreateParticle(int index, float currentTime) {
	if (index >= MAX_PARTICLE_COUNT) index -= MAX_PARTICLE_COUNT;
	particles[index].emitTime = currentTime;
	particles[index].startPosition = position;
	printf("Particle at index %i created\n", index);
}

void Emitter::UpdateExistingParticles(float currentTime) {
	int index;
	printf("\r-------CURRENT PARTICLES-------\n");
	for (int i = firstLivingParticle; i < firstLivingParticle + currentParticleCount; i++) {
		// Account for "ring buffer" structure
		// if our loop index exceeds max particles, our reference index will reset to 0 to account for this.
		index = i >= MAX_PARTICLE_COUNT ? i - MAX_PARTICLE_COUNT : i;
		float particleAge = currentTime - particles[index].emitTime;
		printf("\rParticle at index %i: Age: %f\n", index, particleAge);
		if (particleAge > particleLifetime) {
			firstLivingParticle++;
			currentParticleCount--;
			printf("Particle at index %i deleted\n", index);
			LoopParticleIndices();
		}
	}
}

void Emitter::LoopParticleIndices() {
	if (firstLivingParticle >= MAX_PARTICLE_COUNT) firstLivingParticle = 0;
	if (firstDeadParticle >= MAX_PARTICLE_COUNT) firstDeadParticle = 0;
}

void Emitter::CopyAllBufferData(XMFLOAT4X4 view, XMFLOAT4X4 projection, float currentTime) {
	// Start by mapping
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	Graphics::Context->Map(particleDataBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	// Copy everything. Depends on particle arrangement
	if (firstLivingParticle < firstDeadParticle) {
		// Memory location is straight, so simply copy from firstLiving to firstDead
		memcpy(mapped.pData, particles + firstLivingParticle, sizeof(Particle) * currentParticleCount);
	}
	else {
		// Copy from 0 to firstDead
		memcpy(mapped.pData, particles, sizeof(Particle) * firstDeadParticle);

		// Copy again from firstLiving to the end
		memcpy((void*)((Particle*)mapped.pData + firstDeadParticle), particles + firstLivingParticle, sizeof(Particle) * (MAX_PARTICLE_COUNT - firstLivingParticle));
	}
	
	// Now that we're done, unmap the buffer
	Graphics::Context->Unmap(particleDataBuffer.Get(), 0);

	vertexShader->SetMatrix4x4("view", view);
	vertexShader->SetMatrix4x4("projection", projection);
	vertexShader->SetFloat4("colorTint", particleColor);
	vertexShader->SetFloat("currentTime", currentTime);
	vertexShader->SetShaderResourceView("particleData", particleDataSRV);
	vertexShader->CopyAllBufferData();

	pixelShader->SetShaderResourceView("ParticleTexture", textureSRV);
	pixelShader->SetSamplerState("Sampler", samplerState);
	pixelShader->CopyAllBufferData();
}