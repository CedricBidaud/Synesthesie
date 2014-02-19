#ifndef _IMAC3_BRAKE_FORCE_HPP
#define _IMAC3_BRAKE_FORCE_HPP

#include <iostream>

#include "Force.hpp"
#include "Leapfrog.hpp"

namespace imac3 {

class BrakeForce : Force {

	public:
		BrakeForce(float dt, float v, float l, const Leapfrog& solver);
		void apply(ParticleManager &pm);
		void setDt(float dt);
	
	public: // getter setter à faire
		float m_fDt;
		float m_fV;
		float m_fL;
		float m_fAmort;
		
	private:
		const Leapfrog* m_solver;
};

}



#endif // _IMAC3_BRAKE_FORCE_HPP
