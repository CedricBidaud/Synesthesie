#include "ConstantForce.hpp"

namespace imac3 {

	ConstantForce::ConstantForce(float fK, float fL){
		m_fK = fK;
		m_fL = fL;
	}

	void ConstantForce::apply(ParticleManager &pm) {
		unsigned int count = pm.getSize();
		for(unsigned int i = 0; i < count; ++i){
			glm::vec2 pos = pm.getParticlePosition(i);
			
			glm::vec2 p2 = glm::vec2(0.,0.);
					
			glm::vec2 vect = p2 - pos;
			
			float norm = glm::length(vect);
			
			glm::vec2 force = glm::vec2(0.0);
			
			// Répulsion
			//~ if(norm < m_fLInf - 0.04f){
				//~ force = m_fKRep*(1-(m_fLInf-0.04f)/glm::max(norm,0.0001f))*(vect);
			//~ }
			
			// Sticky
			//~ if(norm > m_fLInf && norm < m_fLSup){
				force = m_fK*(1-m_fL/glm::max(norm,0.0001f))*(vect);
			//~ }
			
			// tests compression
			//~ if(vect.y < 0.0f){
				//~ force *= (2.0f-vect.y)*0.5f;
			//~ }
			//~ if(vect.y < 0.0f){
				//~ force *= vect.y;
			//~ }
				
			pm.addForce(i, force);
			
			
			//~ pm.addForce(i, -pos);
		}
	}

}
