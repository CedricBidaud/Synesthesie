#ifndef _IMAC3_POLYGON_HPP
#define _IMAC3_POLYGON_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/ParticleRenderer2D.hpp"

namespace imac3{

	class Polygon{
		public:
			static Polygon buildBox(glm::vec3 color, glm::vec2 position, float width, float height, bool isInner = false);
			static Polygon buildBox(glm::vec3 color, glm::vec2 position, float width, float height, float rotationInDegrees);
			static Polygon buildDefaultBox(glm::vec3 color);
			static Polygon buildCircle(glm::vec3 color, glm::vec2 center, float radius, size_t discFactor, bool isInner = false);
			
			void draw(ParticleRenderer2D& renderer, float lineWidth = 1.f) const;
			unsigned int getSize() const;
			glm::vec2 getPosition(int idx) const;
			glm::vec2 center;
			float rotationInDegrees = 0.0f;
			bool isInner() const;
			
			void rotate(float angleInDegrees);
			void scale(glm::vec2 s);
			void translate(glm::vec2 t);
			
			std::vector<glm::vec2> m_pointsArray;
	
		private:
			glm::vec3 m_color;
			bool m_isInner;
	};

}

#endif // _IMAC3_POLYGON_HPP
