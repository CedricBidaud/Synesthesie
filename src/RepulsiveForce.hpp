#ifndef _IMAC3_REP_FORCE_HPP
#define _IMAC3_REP_FORCE_HPP


#include <iostream>

#include "Force.hpp"

namespace imac3 {

class RepulsiveForce : Force {

	public:
		RepulsiveForce(float fKRep, float fKSticky, float fLInf, float fLSup);
		void apply(ParticleManager &pm);
	
	public: // getter setter à faire
		float m_fKRep;
		float m_fKSticky;
		float m_fLInf;
		float m_fLSup;

};

}



#endif // _IMAC3_REP_FORCE_HPP
