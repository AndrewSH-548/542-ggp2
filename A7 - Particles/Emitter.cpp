#include <iostream>

#include "Emitter.h"

using namespace std;

void Emitter::Update(float delta, float currentTime) {
	// Progress in time
	timeSinceLastEmission += delta;

	// Emit a particle 
	if (timeSinceLastEmission >= secondsBetweenParticles) {
		if (currentParticleCount < maxParticleCount) {
			
			// Add a space to the chunk of alive particles
			currentParticleCount++;
			firstDeadParticle++;
			LoopParticleIndices();
			CreateParticle(firstLivingParticle + currentParticleCount, currentTime);
		}
		UpdateExistingParticles(currentTime);

		timeSinceLastEmission = 0;
	}
}

void Emitter::CreateParticle(int index, float currentTime) {
	particles[index].emitTime = currentTime;
	particles[index].startPosition = position;
	cout << "Particle at index " << index << " created" << endl;
}

void Emitter::UpdateExistingParticles(float currentTime) {
	int index;
	cout << "-------CURRENT PARTICLES-------" << endl;
	for (int i = firstLivingParticle; i < firstLivingParticle + currentParticleCount; i++) {
		// Account for "ring buffer" structure
		// if our loop index exceeds max particles, our reference index will reset to 0 to account for this.
		index = i > maxParticleCount ? i - maxParticleCount : i;
		float particleAge = currentTime - particles[index].emitTime;
		cout << "Particle at index " << index << ": Age: " << particleAge << endl;
		if (particleAge > particleLifetime) {
			firstLivingParticle++;
			cout << "Particle at index " << index << " deleted" << endl;
			LoopParticleIndices();
		}
	}
}

void Emitter::LoopParticleIndices() {
	if (firstLivingParticle > maxParticleCount) firstLivingParticle = 0;
	if (firstDeadParticle > maxParticleCount) firstDeadParticle = 0;
}