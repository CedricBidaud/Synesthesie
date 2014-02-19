#ifndef _IMAC3_LEAPFROG_HPP
#define _IMAC3_LEAPFROG_HPP

#include <iostream>

#include "ParticleManager.hpp"

namespace imac3{

	class Leapfrog{
		private:
			
		
		public:
			struct ParticleState{
				glm::vec2 position;
				glm::vec2 velocity;
			};
			
			void solve(ParticleManager& pm, float dt);
			static ParticleState getNextState(uint32_t particleIdx, ParticleManager& pm, float dt);
	
	};

}


#endif // _IMAC3_LEAPFROG_HPP

