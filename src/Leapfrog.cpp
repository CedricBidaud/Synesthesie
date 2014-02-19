#include "Leapfrog.hpp"

namespace imac3 {
	void Leapfrog::solve(ParticleManager& pm, float dt){
		glm::vec2 force, position, velocity;
		float mass;
		for(int i = 0; i < pm.getSize(); ++i){
			position = pm.getParticlePosition(i);
			force = pm.getParticleForce(i);
			velocity = pm.getParticleVelocity(i);
			mass = pm.getParticleMass(i);
			
			glm::vec2 nextVelocity = velocity + dt*(force/mass);
			glm::vec2 nextPosition = position + dt*nextVelocity;

			pm.setParticlePosition(i, nextPosition);
			pm.setParticleVelocity(i, nextVelocity);

			pm.resetParticleForce(i);
		}
	}
	
	Leapfrog::ParticleState Leapfrog::getNextState(uint32_t particleIdx, ParticleManager& pm, float dt){
		ParticleState state;
		
		glm::vec2 force, position, velocity;
		float mass;
		
		position = pm.getParticlePosition(particleIdx);
		force = pm.getParticleForce(particleIdx);
		velocity = pm.getParticleVelocity(particleIdx);
		mass = pm.getParticleMass(particleIdx);
		
		glm::vec2 nextVelocity = velocity + dt*(force/mass);
		glm::vec2 nextPosition = position + dt*nextVelocity;
		
		state.velocity = nextVelocity;
		state.position = nextPosition;
		
		return state;
		
	}
}
