#ifndef _IMAC3_PARTICLEMANAGER_HPP
#define _IMAC3_PARTICLEMANAGER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/random.hpp>

#include <iostream>

#include "renderer/ParticleRenderer2D.hpp"

namespace imac3 {

class ParticleManager{
	public:
		ParticleManager();
		~ParticleManager();
		unsigned int addParticle(glm::vec2 position, float mass, glm::vec3 color, glm::vec2 velocity);
		void addRandomParticles(unsigned int count);
		unsigned int getSize();
		void addForce(int i, glm::vec2 force);
		glm::vec2 getParticlePosition(unsigned int i);
		glm::vec2 getParticleVelocity(unsigned int i);
		float getParticleMass(unsigned int i);
		glm::vec2 getParticleForce(unsigned int i);
		bool decreaseParticleLifetime(unsigned int i);

		void setParticlePosition(unsigned int i, glm::vec2 position);
		void setParticleVelocity(unsigned int i, glm::vec2 velocity);
		void setParticleForce(unsigned int i, glm::vec2 force);
		void killParticle(unsigned int i);
		void resetParticleForce(unsigned int i);
		
		void setColorFromVolume();
		
		void clean();

		void printForces();
		
		void drawParticles(ParticleRenderer2D& renderer);
		void drawParticles(ParticleRenderer2D& renderer, float size, float volume = 0.);
		
		
	private:
		std::vector<glm::vec2> m_positionArray;
		std::vector<glm::vec2> m_velocityArray;
		std::vector<float> m_massArray;
		std::vector<glm::vec2> m_forceArray;
		std::vector<glm::vec3> m_colorArray;
		std::vector<unsigned int> m_lifeTime;
		std::vector<unsigned int> m_instrument;

};

}

#endif
