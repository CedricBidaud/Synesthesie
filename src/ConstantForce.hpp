#ifndef _IMAC3_CONST_FORCE_HPP
#define _IMAC3_CONST_FORCE_HPP


#include <iostream>

#include "Force.hpp"

namespace imac3 {

class ConstantForce : Force {

	public:
		ConstantForce(float fK, float fL);
		void apply(ParticleManager &pm);
		glm::vec2 m_force;

		float m_fK;
		//~ float m_fKSticky;
		float m_fL;
};

}



#endif // _IMAC3_CONST_FORCE_HPP
