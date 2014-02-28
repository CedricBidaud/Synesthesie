#include "Instrument.hpp"

namespace imac3{

	Instrument::Instrument(glm::vec2 position, glm::vec2 velocity, int index, glm::vec3 lowColor, glm::vec3 highColor, int lowBound, int highBound, ParticleManager* manager){
		m_position = position;
		m_velocity = velocity;
		m_index = index;
		m_lowColor = lowColor;
		m_highColor = highColor;
		m_lowBound = lowBound;
		m_highBound = highBound;
		m_manager = manager;
		open=false;
	}

	Instrument::~Instrument(){
		
	}

	unsigned int Instrument::addParticle() {		
		m_manager->addRandomParticle(m_position, m_velocity, m_lowColor, m_index);
	}
	
	glm::vec3 Instrument::giveColor(glm::vec3 baseColor, float scaledVolume){
		return glm::vec3(scaledVolume,scaledVolume,scaledVolume);
	}

} // end namespace imac3
