#ifndef _IMAC3_POLY_FORCE_HPP
#define _IMAC3_POLY_FORCE_HPP


#include <iostream>

#include "Force.hpp"
#include "Polygon.hpp"
#include "Leapfrog.hpp"

namespace imac3 {

class PolygonForce : Force {

	public:
		PolygonForce(const Polygon& polygon, float elasticity, const Leapfrog& solver);
		void setDt(float dt);
		void apply(ParticleManager &pm);
		static glm::vec2 computeForce(float elasticity, glm::vec2 velocity, glm::vec2 normale, float mass, float dt);
	
	private:
		const Polygon* m_Polygon;
		float m_fElasticity;
		const Leapfrog* m_Solver;
		float m_fDt;

};

}



#endif // _IMAC3_POLY_FORCE_HPP

