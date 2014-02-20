#ifndef _IMAC3_UNIV_FORCE_HPP
#define _IMAC3_UNIV_FORCE_HPP


#include <iostream>

#include "Force.hpp"
#include "Leapfrog.hpp"

namespace imac3 {

class UniversalForce : Force {

	public:
		UniversalForce(const Leapfrog& solver, float fKRep, float fKSticky, float fLInf, float fLSup, float fConstK, float fConstL, float dt, float brakeV, float brakeL);
		void apply(ParticleManager &pm);
		void setDt(float dt);
	
	public: // getter setter à faire
		// repulsion, sticky
		float m_fKRep;
		float m_fKSticky;
		float m_fLInf;
		float m_fLSup;
		
		// attraction centre
		float m_fConstK;
		float m_fConstL;
		
		// brake
		float m_fDt;
		float m_fBrakeV;
		float m_fBrakeL;
		float m_fBrakeAmort;
		
	private:
		const Leapfrog* m_solver;

};

}



#endif // _IMAC3_UNIV_FORCE_HPP
