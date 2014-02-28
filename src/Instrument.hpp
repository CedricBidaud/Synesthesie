#ifndef _IMAC3_INSTRUMENT_HPP
#define _IMAC3_INSTRUMENT_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/random.hpp>

#include <iostream>

#include "ParticleManager.hpp"

namespace imac3 {

class Instrument{
	public:
		Instrument(glm::vec2 position, glm::vec2 velocity, int index, glm::vec3 lowColor, glm::vec3 highColor, int lowBound, int highBound, ParticleManager* manager);
		~Instrument();
		unsigned int addParticle();
	
		bool open;
		enum e_instrument { drums, bass, guitarA, guitarB };
		
		static glm::vec3 giveColor(glm::vec3 baseColor, float scaledVolume);
		
	private:
		glm::vec3 m_lowColor;
		glm::vec3 m_highColor;
		int m_lowBound; //Hz
		int m_highBound; //Hz 
		glm::vec2 m_position;
		glm::vec2 m_velocity;
		int m_index;
		ParticleManager* m_manager;
		
};

}

#endif
