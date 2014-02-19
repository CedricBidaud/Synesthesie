#include <cmath>

#include "HookForce.hpp"

namespace imac3 {

	HookForce::HookForce(float fK, float fL){
		m_fK= fK;
		m_fL= fL;
	}

	void HookForce::apply(ParticleManager &pm) {
		unsigned int count = pm.getSize();

		for(int i = 0; i < count; ++i){

			glm::vec2 p1=pm.getParticlePosition(i);

			for(int j = 0; j < count; ++j){
				if(i != j){
					glm::vec2 p2=pm.getParticlePosition(j);
					
					float norm = sqrt(glm::dot(p2-p1,p2-p1));
					
					glm::vec2 force = glm::vec2(0.0);
					
					if(norm < 0.1)
						force = m_fK*(1-m_fL/glm::max(norm,0.0001f))*(p2-p1);
						
					pm.addForce(i, force);
				}
			}

		}
	}

}
