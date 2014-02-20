#include <cmath>

#include "UniversalForce.hpp"

namespace imac3 {

	UniversalForce::UniversalForce(const Leapfrog& solver, float fKRep, float fKSticky, float fLInf, float fLSup, float fConstK, float fConstL, float dt, float brakeV, float brakeL){
		m_fKRep= fKRep;
		m_fKSticky= fKSticky;
		m_fLInf= fLInf;
		m_fLSup= fLSup;
		m_fConstK = fConstK;
		m_fConstL = fConstL;
		m_fDt = dt;
		m_fBrakeV = brakeV;
		m_fBrakeL = brakeL;
		m_fBrakeAmort = 0.000f;
		m_solver = &solver;
	}
	
	void UniversalForce::setDt(float dt){
		m_fDt = dt;
	}

	void UniversalForce::apply(ParticleManager &pm) {
		unsigned int count = pm.getSize();
		
		for(int i = 0; i < count; ++i){
			// ---------------------
			// ----- Répulsion -----
			// ---------------------
			glm::vec2 p1=pm.getParticlePosition(i);

			for(int j = 0; j < count; ++j){
				if(i != j){
					glm::vec2 p2=pm.getParticlePosition(j);
					
					glm::vec2 vect = p2 - p1;
					
					float norm = glm::length(vect);
					
					glm::vec2 force = glm::vec2(0.0);
					
					// Répulsion
					if(norm < m_fLInf - 0.04f){
						force = m_fKRep*(1-(m_fLInf-0.04f)/glm::max(norm,0.0001f))*(vect);
					}
					
					// Sticky
					if(norm > m_fLInf && norm < m_fLSup){
						force = m_fKSticky*(1-m_fLInf/glm::max(norm,0.0001f))*(vect);
					}
						
					pm.addForce(i, force);
				}
			}
			// Fin répulsion
			
			// ----------------------------------------
			// - Attraction vers le centre de l'écran -
			// ----------------------------------------
			glm::vec2 pos = pm.getParticlePosition(i);
			glm::vec2 p2 = glm::vec2(0.,0.);
			glm::vec2 vect = p2 - pos;
			
			float norm = glm::length(vect);
			
			glm::vec2 force = glm::vec2(0.0);
			force = m_fConstK*(1-m_fConstL/glm::max(norm,0.0001f))*(vect);
			pm.addForce(i, force);
			// Fin attraction centre
			
			
			// -----------------
			// ----- Brake -----
			// -----------------
			glm::vec2 v1=pm.getParticleVelocity(i);

			for(int j = 0; j < count; ++j){
				if(i != j){
					glm::vec2 v2 = pm.getParticleVelocity(j);
					glm::vec2 p1 = pm.getParticlePosition(i);
					glm::vec2 p2 = pm.getParticlePosition(j);

					if(m_fDt > 0){
						glm::vec2 force = glm::vec2(0.0, 0.0);
						
						float norm = glm::length(p2-p1);
						
						if(norm < m_fBrakeL){
							force = m_fBrakeV*((v2-v1)/m_fDt);
						}
						
						// amortissement
						if(m_fBrakeAmort > 0.0f){
							glm::vec2 amortissment = glm::vec2(0.0);
							
							Leapfrog::ParticleState actualState;
							actualState.position = pm.getParticlePosition(i);
							actualState.velocity = pm.getParticleVelocity(i);
							
							Leapfrog::ParticleState nextState = Leapfrog::getNextState(i, pm, m_fDt);
							
							if(m_fDt > 0.0){ 
								glm::vec2 dV = (nextState.velocity - actualState.velocity) / m_fDt;
								
								dV *= m_fBrakeAmort;
								
								pm.addForce(i, -dV);
							}
						}
						
						pm.addForce(i, force);

					}
				}
			}
			// fin Brake
			
		}
	}

}

