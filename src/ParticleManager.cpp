#include "ParticleManager.hpp"
#include "renderer/GLtools.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace imac3{

ParticleManager::ParticleManager(){
}

ParticleManager::~ParticleManager(){
	
}

unsigned int ParticleManager::addParticle(glm::vec2 position, float mass, glm::vec3 color, glm::vec2 velocity, unsigned int instrument) {
	m_positionArray.push_back(position);
	m_massArray.push_back(mass);
	m_colorArray.push_back(color);
	m_velocityArray.push_back(velocity);
	m_forceArray.push_back(glm::vec2(0.f));
	m_lifeTime.push_back(100);
	m_instrument.push_back(instrument);
	return m_positionArray.size() - 1;
	
	
}

void ParticleManager::addRandomParticles(unsigned int count, unsigned int instrument){
	for (int i=0;i<count;++i){
		glm::vec2 pos = glm::vec2(glm::linearRand(0.5f,0.5f),glm::linearRand(0.75f,0.75f));
		
		unsigned int nb = this->getSize()*4;
		float r = 0.0f;
		float g = 0.8f;
		float b = 0.5f;
		
		//~ r = nb / 255.0f;
		//~ g = (int(nb / 255.0f) * 25.0f) / 255.0f;
		//~ b = (g * 100.0f) / 255.0f;
		
		glm::vec2 direction = glm::vec2(glm::linearRand(-0.03,-0.05),glm::linearRand(0.05,0.10));
		float mass = 1.0f;
		glm::vec3 color = glm::vec3(r,g,b);
		
		addParticle(pos, mass, color, direction, instrument);
	}
}

void ParticleManager::addRandomParticle(glm::vec2 position, glm::vec2 velocity, unsigned int instrument){
	
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	
	float mass = 1.0f;
	glm::vec3 color = glm::vec3(r,g,b);
	
	addParticle(position, mass, color, velocity, instrument);
}


void ParticleManager::drawParticles(ParticleRenderer2D& renderer) {
	renderer.drawParticles(
		m_positionArray.size(),
		&m_positionArray[0],
		&m_massArray[0],
		&m_colorArray[0]
	);
}
void ParticleManager::drawParticles(ParticleRenderer2D& renderer, float size, float volume) {
	renderer.drawParticles(
		m_positionArray.size(),
		&m_positionArray[0],
		&m_massArray[0],
		&m_colorArray[0],
		size,
		volume
	);
}

unsigned int ParticleManager::getSize() {
	return m_positionArray.size();
}

void ParticleManager::addForce(int i, glm::vec2 force){
	m_forceArray[i] += force;
	//~ std::cout << "force : " << m_forceArray[i].x << ", " << m_forceArray[i].y << std::endl;
}

void ParticleManager::printForces(){
	for (int i=0;i<m_forceArray.size();++i){
		//~ std::cout << "force : " << m_forceArray[i].x << ", " << m_forceArray[i].y << std::endl;
	}
}

float ParticleManager::getParticleMass(unsigned int i){
	return m_massArray[i];
}

glm::vec2 ParticleManager::getParticlePosition(unsigned int i){
	return m_positionArray[i];
}

glm::vec2 ParticleManager::getParticleVelocity(unsigned int i){
	return m_velocityArray[i];
}

glm::vec2 ParticleManager::getParticleForce(unsigned int i){
	return m_forceArray[i];
}

void ParticleManager::setParticlePosition(unsigned int i, glm::vec2 position){
	m_positionArray[i] = position;
}

void ParticleManager::setParticleVelocity(unsigned int i, glm::vec2 velocity){
	m_velocityArray[i] = velocity;
}

void ParticleManager::setParticleForce(unsigned int i, glm::vec2 force){
	m_forceArray[i] = force;
}

void ParticleManager::resetParticleForce(unsigned int i){
	//~ std::cout << "Before : " << m_forceArray[i].x << ", " << m_forceArray[i].y << std::endl;
	setParticleForce(i, glm::vec2(0.f));
	//~ std::cout << "After : " << m_forceArray[i].x << ", " << m_forceArray[i].y << std::endl;
}


bool ParticleManager::decreaseParticleLifetime(unsigned int i) {
	//~ std::cout << "lifetime : " << m_lifeTime[i] << std::endl;
	return (--m_lifeTime[i] == 0); 
}

void ParticleManager::killParticle(unsigned int i){
	m_positionArray.erase(m_positionArray.begin()+i);
	m_massArray.erase(m_massArray.begin()+i);
	m_colorArray.erase(m_colorArray.begin()+i);
	m_velocityArray.erase(m_velocityArray.begin()+i);
	m_forceArray.erase(m_forceArray.begin()+i);
	m_lifeTime.erase(m_lifeTime.begin()+i);
	m_instrument.erase(m_instrument.begin()+i);
}


void ParticleManager::clean(){
	m_positionArray.clear();
	m_massArray.clear();
	m_colorArray.clear();
	m_velocityArray.clear();
	m_forceArray.clear();
	m_lifeTime.clear();
	m_instrument.clear();
}

} // end namespace imac3
