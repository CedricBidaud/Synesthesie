#ifndef _IMAC3_HOOK_FORCE_HPP
#define _IMAC3_HOOK_FORCE_HPP


#include <iostream>

#include "Force.hpp"

namespace imac3 {

class HookForce : Force {

	public:
		HookForce(float fK, float fL);
		void apply(ParticleManager &pm);
	
	private:
		float m_fK;
		float m_fL;

};

}



#endif // _IMAC3_HOOK_FORCE_HPP