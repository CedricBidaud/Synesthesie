#ifndef _IMAC3_GLTOOLS_HPP
#define _IMAC3_GLTOOLS_HPP

#include <GL/glew.h>

#define GL_STRINGIFY(s) #s

namespace imac3 {

GLuint buildProgram(const GLchar* vertexShaderSrc, const GLchar* fragmentShaderSrc);

}

#endif // _IMAC3_GLTOOLS_HPP
