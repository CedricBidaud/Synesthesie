#include "Instrument.hpp"

namespace imac3{

	Instrument::Instrument(glm::vec2 position, glm::vec2 velocity, int index, ParticleManager* manager){
		m_position = position;
		m_velocity = velocity;
		m_index = index;
		m_manager = manager;
		open=false;
	}

	Instrument::~Instrument(){
		
	}

	unsigned int Instrument::addParticle() {		
		m_manager->addRandomParticle(m_position, m_velocity, m_index);
	}

} // end namespace imac3
