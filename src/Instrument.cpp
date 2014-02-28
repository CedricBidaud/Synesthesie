#include "Instrument.hpp"

namespace imac3{

	Instrument::Instrument(glm::vec2 position, glm::vec2 velocity, int index, ParticleManager* manager){
		m_position = position;
		m_velocity = velocity;
		m_index = index;
		m_manager = manager;
	}

	Instrument::~Instrument(){
		
	}

	unsigned int Instrument::addParticle() {		
		m_manager->addRandomParticle(m_position, m_velocity, m_index);
	}
	
	glm::vec3 Instrument::giveColor(glm::vec3 baseColor, float scaledVolume){
		return glm::vec3(scaledVolume,scaledVolume,scaledVolume);
	}

} // end namespace imac3
