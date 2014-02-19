#ifndef _IMAC3_FORCE_HPP
#define _IMAC3_FORCE_HPP

#include <glm/glm.hpp>
#include <vector>

#include "ParticleManager.hpp"

namespace imac3 {

class Force {

	public:
		virtual ~Force();
		virtual void apply(ParticleManager &pm) = 0; // "= 0" <=> virtuelle pure
		
	private:
		


};



}



#endif // _IMAC3_FORCE_HPP
