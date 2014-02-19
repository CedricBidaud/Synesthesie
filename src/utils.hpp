#ifndef _IMAC3_UTILS_HPP
#define _IMAC3_UTILS_HPP

#include <glm/glm.hpp>

namespace imac3{
	
		glm::vec2 normale(const glm::vec2& A, const glm::vec2& B){
			return glm::normalize(glm::vec2(-(B.y - A.y), (B.x - A.x))); // pas facile à lire sur le tableau...
		}
		
		float squareNorm(glm::vec2 vect){
			return (vect.x*vect.x + vect.y*vect.y);
		}

		float norm(glm::vec2 vect){
			return sqrt(glm::dot(vect,vect));
		}
		
		bool intersect(const glm::vec2& p1, const glm::vec2& p2, 
						const glm::vec2& A, const glm::vec2& B,
						glm::vec2* intersection, glm::vec2* normal){
			glm::vec2 n = normale(A,B);
			
			float k = -glm::dot(p1-A,n)/glm::dot(p2-p1,n);
			
			if(k >= 0 && k <= 1){
				glm::vec2 M = p1+k*(p2-p1);
				
				float scal = glm::dot(M-A, B-A);
				float offset = 0.000;
				
				if(scal >= 0.0f -offset && scal <= glm::dot(B-A, B-A) +offset){
					*intersection = M;
					*normal = n;
					return true;
				}				
			}
			
			return false;
		}
		
}

#endif
