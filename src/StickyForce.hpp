#ifndef _IMAC3_STICKY_FORCE_HPP
#define _IMAC3_STICKY_FORCE_HPP


#include <iostream>

#include "Force.hpp"

namespace imac3 {

class StickyForce : Force {

	public:
		StickyForce(float fK, float fLInf, float fLSup);
		void apply(ParticleManager &pm);
	
	public: // getter setter à faire
		float m_fK;
		float m_fLSup;
		float m_fLInf;

};

}



#endif // _IMAC3_STICKY_FORCE_HPP
