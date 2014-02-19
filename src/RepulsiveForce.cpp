#include <cmath>

#include "RepulsiveForce.hpp"

namespace imac3 {

	RepulsiveForce::RepulsiveForce(float fKRep, float fKSticky, float fLInf, float fLSup){
		m_fKRep= fKRep;
		m_fKSticky= fKSticky;
		m_fLInf= fLInf;
		m_fLSup= fLSup;
		
	}

	void RepulsiveForce::apply(ParticleManager &pm) {
		unsigned int count = pm.getSize();

		for(int i = 0; i < count; ++i){

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
					
					// tests compression
					//~ if(vect.y < 0.0f){
						//~ force *= (2.0f-vect.y)*0.5f;
					//~ }
					//~ if(vect.y < 0.0f){
						//~ force *= vect.y;
					//~ }
						
					pm.addForce(i, force);
				}
			}

		}
	}

}
