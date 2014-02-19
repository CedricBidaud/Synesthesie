#ifndef _IMAC3_PARTICLERENDERER2D_HPP
#define _IMAC3_PARTICLERENDERER2D_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace imac3 {

class ParticleRenderer2D {
public:
    ParticleRenderer2D(float massScale = 0.05);
    ParticleRenderer2D(GLuint particleProgram, GLuint polyProgram, GLuint quadProgram, float massScale = 0.05);

    ~ParticleRenderer2D();

    ParticleRenderer2D(const ParticleRenderer2D&) = delete;

    ParticleRenderer2D& operator =(const ParticleRenderer2D&) = delete;

    void clear();

    void drawParticles(uint32_t count,
                       const glm::vec2* positionArray,
                       const float* massArray,
                       const glm::vec3* colorArray);
                       
    void drawParticles(uint32_t count,
                       const glm::vec2* positionArray,
                       const float* massArray,
                       const glm::vec3* colorArray,
                       float size);

    void drawPolygon(uint32_t count,
                     const glm::vec2* position,
                     const glm::vec3& color,
                     float lineWidth = 1.f);
                     
    void drawQuad(GLuint vao, GLuint texture, int quadTriangleCount, float blurSize, int passe);

private:
    static const GLchar *VERTEX_SHADER, *FRAGMENT_SHADER;
    static const GLchar *POLYGON_VERTEX_SHADER, *POLYGON_FRAGMENT_SHADER;
    static const GLchar *QUAD_VERTEX_SHADER, *QUAD_FRAGMENT_SHADER;

    // Ressources OpenGL
    GLuint m_ProgramID, m_PolygonProgramID, m_QuadProgramID;
    GLuint m_VBOID, m_VAOID;

    GLuint m_PolygonVBOID, m_PolygonVAOID;

    // Uniform locations
    GLint m_uParticleColor;
    GLint m_uParticlePosition;
    GLint m_uParticleScale;

    GLint m_uPolygonColor;
    
    GLint m_uQuadTexture;
    GLint m_uQuadBlurSize;
    GLint m_uQuadPasse;

    // Scale appliqué sur la masse de chaque particule pour obtenir sa taille
    float m_fMassScale;
};

}

#endif // _IMAC3_PARTICLERENDERER2D_HPP
