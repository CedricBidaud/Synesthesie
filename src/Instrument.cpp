#include "Instrument.hpp"

namespace imac3{

	//~ Instrument::Instrument(glm::vec2 position, glm::vec2 velocity, int index, glm::vec3 lowColor, glm::vec3 highColor, int lowBound, int highBound, ParticleManager* manager){
	Instrument::Instrument(glm::vec2 position, glm::vec2 velocity, int index, glm::vec3 lowColor, glm::vec3 highColor, int lowBound, int highBound){
		m_position = position;
		m_velocity = velocity;
		m_index = index;
		m_lowColor = lowColor;
		m_highColor = highColor;
		m_lowBound = lowBound;
		m_highBound = highBound;
		open=false;
	}

	Instrument::~Instrument(){
		
	}

	//~ unsigned int Instrument::addParticle() {		
		//~ m_manager->addRandomParticle(m_position, m_velocity, m_lowColor, m_index);
	//~ }
	
	glm::vec3 Instrument::giveColor(float scaledVolume){
		return ((m_highColor - m_lowColor)*scaledVolume + m_lowColor);
	}

} // end namespace imac3
