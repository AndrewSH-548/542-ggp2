#include <iostream>

#include "Emitter.h"

using namespace std;

void Emitter::Update(float delta, float currentTime) {
	// Progress in time
	timeSinceLastEmission += delta;
	printf("time: %f first", timeSinceLastEmission);
	UpdateExistingParticles(currentTime);

	// Emit a particle 
	if (timeSinceLastEmission >= secondsBetweenParticles) {
		if (currentParticleCount < maxParticleCount) {
			CreateParticle(firstLivingParticle + currentParticleCount, currentTime);

			// Add a space to the chunk of alive particles
			currentParticleCount++;
			firstDeadParticle++;
			LoopParticleIndices();
			
		}

		timeSinceLastEmission = 0;
	}
}

void Emitter::CreateParticle(int index, float currentTime) {
	if (index > 20) index -= 20;
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
		index = i > maxParticleCount ? i - maxParticleCount : i;
		float particleAge = currentTime - particles[index].emitTime;
		printf("Particle at index %i: Age: %f\n", index, particleAge);
		if (particleAge > particleLifetime) {
			firstLivingParticle++;
			currentParticleCount--;
			printf("Particle at index %i deleted\n", index);
			LoopParticleIndices();
		}
	}
}

void Emitter::LoopParticleIndices() {
	if (firstLivingParticle > maxParticleCount) firstLivingParticle = 0;
	if (firstDeadParticle > maxParticleCount) firstDeadParticle = 0;
}