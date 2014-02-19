#include "ParticleRenderer2D.hpp"
#include "GLtools.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace imac3 {

// shaders écrits dans des fichiers externes
ParticleRenderer2D::ParticleRenderer2D(GLuint particleProgram, GLuint polyProgram, GLuint quadProgram, float massScale):
    m_ProgramID(particleProgram),
    m_PolygonProgramID(polyProgram),
    m_QuadProgramID(quadProgram),
    m_fMassScale(massScale) {

    // RÃ©cuperation des uniforms
    m_uParticleColor = glGetUniformLocation(m_ProgramID, "uParticleColor");
    m_uParticlePosition = glGetUniformLocation(m_ProgramID, "uParticlePosition");
    m_uParticleScale = glGetUniformLocation(m_ProgramID, "uParticleScale");

    m_uPolygonColor = glGetUniformLocation(m_PolygonProgramID, "uPolygonColor");
    
    m_uQuadTexture = glGetUniformLocation(m_QuadProgramID, "Texture");
    m_uQuadBlurSize = glGetUniformLocation(m_QuadProgramID, "uBlurSize");
   
    // CrÃ©ation du VBO
    glGenBuffers(1, &m_VBOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);

    // Une particule est un carrÃ©
    GLfloat positions[] = {
        -1.f, -1.f,
         1.f, -1.f,
         1.f,  1.f,
        -1.f,  1.f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    // CrÃ©ation du VAO
    glGenVertexArrays(1, &m_VAOID);
    glBindVertexArray(m_VAOID);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenBuffers(1, &m_PolygonVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, m_PolygonVBOID);
    glGenVertexArrays(1, &m_PolygonVAOID);
    glBindVertexArray(m_PolygonVAOID);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ParticleRenderer2D::~ParticleRenderer2D() {
    glDeleteBuffers(1, &m_VBOID);
    glDeleteBuffers(1, &m_PolygonVBOID);
    glDeleteVertexArrays(1, &m_VAOID);
    glDeleteVertexArrays(1, &m_PolygonVAOID);
}

void ParticleRenderer2D::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void ParticleRenderer2D::drawParticles(
        uint32_t count,
        const glm::vec2* positionArray,
        const float* massArray,
        const glm::vec3* colorArray) {
    // Active la gestion de la transparence
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);

    glUseProgram(m_ProgramID);

    glBindVertexArray(m_VAOID);

    // Dessine chacune des particules
    for(uint32_t i = 0; i < count; ++i) {
        glUniform3fv(m_uParticleColor, 1, glm::value_ptr(colorArray[i]));
        glUniform2fv(m_uParticlePosition, 1, glm::value_ptr(positionArray[i]));
        glUniform1f(m_uParticleScale, m_fMassScale * massArray[i]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

void ParticleRenderer2D::drawParticles(
        uint32_t count,
        const glm::vec2* positionArray,
        const float* massArray,
        const glm::vec3* colorArray,
        float size) {
    // Active la gestion de la transparence
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);

    glUseProgram(m_ProgramID);

    glBindVertexArray(m_VAOID);

    // Dessine chacune des particules
    for(uint32_t i = 0; i < count; ++i) {
        glUniform3fv(m_uParticleColor, 1, glm::value_ptr(colorArray[i]));
        glUniform2fv(m_uParticlePosition, 1, glm::value_ptr(positionArray[i]));
        glUniform1f(m_uParticleScale, m_fMassScale * size);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

void ParticleRenderer2D::drawPolygon(uint32_t count,
                 const glm::vec2* position,
                 const glm::vec3& color,
                 float lineWidth) {
    glBindBuffer(GL_ARRAY_BUFFER, m_PolygonVBOID);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(position[0]), position, GL_DYNAMIC_DRAW);

    glDisable(GL_DEPTH_TEST);

    glLineWidth(lineWidth);

    glUseProgram(m_PolygonProgramID);

    glBindVertexArray(m_PolygonVAOID);

    glUniform3fv(m_uPolygonColor, 1, glm::value_ptr(color));
    glDrawArrays(GL_LINE_LOOP, 0, count);

    glBindVertexArray(0);
}

void ParticleRenderer2D::drawQuad(GLuint vao, GLuint framebuffer, int quadTriangleCount, float blurSize, int passe){
	glUseProgram(m_QuadProgramID);
	
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebuffer);
    
    glUniform1i(m_uQuadTexture, 0);
    glUniform1f(m_uQuadBlurSize, blurSize);
	
	glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, quadTriangleCount*3, GL_UNSIGNED_INT, (void*)0); 
	glBindVertexArray(0);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(0);
}

}
