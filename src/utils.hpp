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
		
		// Shader stuff
		struct ShaderGLSL
		{
			enum ShaderType
			{
				VERTEX_SHADER = 1,
				FRAGMENT_SHADER = 2,
				GEOMETRY_SHADER = 4
			};
			GLuint program;
		};
		
		int  compile_and_link_shader(ShaderGLSL & shader, int typeMask, const char * sourceBuffer, int bufferSize)
		{
			// Create program object
			shader.program = glCreateProgram();
			
			//Handle Vertex Shader
			GLuint vertexShaderObject ;
			if (typeMask & ShaderGLSL::VERTEX_SHADER)
			{
				// Create shader object for vertex shader
				vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
				// Add #define VERTEX to buffer
				const char * sc[3] = { "#version 330\n", "#define VERTEX\n", sourceBuffer};
				glShaderSource(vertexShaderObject, 
							   3, 
							   sc,
							   NULL);
				// Compile shader
				glCompileShader(vertexShaderObject);

				// Get error log size and print it eventually
				int logLength;
				glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &logLength);
				if (logLength > 1)
				{
					char * log = new char[logLength];
					glGetShaderInfoLog(vertexShaderObject, logLength, &logLength, log);
					fprintf(stderr, "Error in compiling vertex shader : %s", log);
					fprintf(stderr, "%s\n%s\n%s", sc[0], sc[1], sc[2]);
					delete[] log;
				}
				// If an error happend quit
				int status;
				glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
				if (status == GL_FALSE)
					return -1;          

				//Attach shader to program
				glAttachShader(shader.program, vertexShaderObject);
			}

			// Handle Geometry shader
			GLuint geometryShaderObject ;
			if (typeMask & ShaderGLSL::GEOMETRY_SHADER)
			{
				// Create shader object for Geometry shader
				geometryShaderObject = glCreateShader(GL_GEOMETRY_SHADER);
				// Add #define Geometry to buffer
				const char * sc[3] = { "#version 330\n", "#define GEOMETRY\n", sourceBuffer};
				glShaderSource(geometryShaderObject, 
							   3, 
							   sc,
							   NULL);
				// Compile shader
				glCompileShader(geometryShaderObject);

				// Get error log size and print it eventually
				int logLength;
				glGetShaderiv(geometryShaderObject, GL_INFO_LOG_LENGTH, &logLength);
				if (logLength > 1)
				{
					char * log = new char[logLength];
					glGetShaderInfoLog(geometryShaderObject, logLength, &logLength, log);
					fprintf(stderr, "Error in compiling Geometry shader : %s \n", log);
					fprintf(stderr, "%s\n%s\n%s", sc[0], sc[1], sc[2]);
					delete[] log;
				}
				// If an error happend quit
				int status;
				glGetShaderiv(geometryShaderObject, GL_COMPILE_STATUS, &status);
				if (status == GL_FALSE)
					return -1;          

				//Attach shader to program
				glAttachShader(shader.program, geometryShaderObject);
			}


			// Handle Fragment shader
			GLuint fragmentShaderObject ;
			if (typeMask && ShaderGLSL::FRAGMENT_SHADER)
			{
				// Create shader object for fragment shader
				fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
				// Add #define fragment to buffer
				const char * sc[3] = { "#version 330\n", "#define FRAGMENT\n", sourceBuffer};
				glShaderSource(fragmentShaderObject, 
							   3, 
							   sc,
							   NULL);
				// Compile shader
				glCompileShader(fragmentShaderObject);

				// Get error log size and print it eventually
				int logLength;
				glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &logLength);
				if (logLength > 1)
				{
					char * log = new char[logLength];
					glGetShaderInfoLog(fragmentShaderObject, logLength, &logLength, log);
					fprintf(stderr, "Error in compiling fragment shader : %s \n", log);
					fprintf(stderr, "%s\n%s\n%s", sc[0], sc[1], sc[2]);
					delete[] log;
				}
				// If an error happend quit
				int status;
				glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
				if (status == GL_FALSE)
					return -1;          

				//Attach shader to program
				glAttachShader(shader.program, fragmentShaderObject);
			}


			// Link attached shaders
			glLinkProgram(shader.program);

			// Clean
			if (typeMask & ShaderGLSL::VERTEX_SHADER)
			{
				glDeleteShader(vertexShaderObject);
			}
			if (typeMask && ShaderGLSL::GEOMETRY_SHADER)
			{
				glDeleteShader(fragmentShaderObject);
			}
			if (typeMask && ShaderGLSL::FRAGMENT_SHADER)
			{
				glDeleteShader(fragmentShaderObject);
			}

			// Get link error log size and print it eventually
			int logLength;
			glGetProgramiv(shader.program, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 1)
			{
				char * log = new char[logLength];
				glGetProgramInfoLog(shader.program, logLength, &logLength, log);
				fprintf(stderr, "Error in linking shaders : %s \n", log);
				delete[] log;
			}
			int status;
			glGetProgramiv(shader.program, GL_LINK_STATUS, &status);        
			if (status == GL_FALSE)
				return -1;


			return 0;
		}

		int  destroy_shader(ShaderGLSL & shader)
		{
			glDeleteProgram(shader.program);
			shader.program = 0;
			return 0;
		}

		int load_shader_from_file(ShaderGLSL & shader, const char * path, int typemask)
		{
			int status;
			FILE * shaderFileDesc = fopen( path, "rb" );
			if (!shaderFileDesc)
				return -1;
			fseek ( shaderFileDesc , 0 , SEEK_END );
			long fileSize = ftell ( shaderFileDesc );
			rewind ( shaderFileDesc );
			char * buffer = new char[fileSize + 1];
			fread( buffer, 1, fileSize, shaderFileDesc );
			buffer[fileSize] = '\0';
			status = compile_and_link_shader( shader, typemask, buffer, fileSize );
			delete[] buffer;
			return status;
		}
		
		void loadShaders(ShaderGLSL * particleShader, ShaderGLSL * polyShader, ShaderGLSL * quadShader){
			const char * shaderFile = "../src/shaders/particle.glsl";
			//~ int status = load_shader_from_file(shader, shaderFile, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER | ShaderGLSL::GEOMETRY_SHADER);
			int status = load_shader_from_file(*particleShader, shaderFile, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER);
			if ( status == -1 )
			{
				fprintf(stderr, "Error on loading  %s\n", shaderFile);
				exit( EXIT_FAILURE );
			}
			
			shaderFile = "../src/shaders/poly.glsl";
			//~ int status = load_shader_from_file(shader, shaderFile, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER | ShaderGLSL::GEOMETRY_SHADER);
			status = load_shader_from_file(*polyShader, shaderFile, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER);
			if ( status == -1 )
			{
				fprintf(stderr, "Error on loading  %s\n", shaderFile);
				exit( EXIT_FAILURE );
			}
						
			shaderFile = "../src/shaders/quad.glsl";
			//~ int status = load_shader_from_file(shader, shaderFile, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER | ShaderGLSL::GEOMETRY_SHADER);
			status = load_shader_from_file(*quadShader, shaderFile, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER);
			if ( status == -1 )
			{
				fprintf(stderr, "Error on loading  %s\n", shaderFile);
				exit( EXIT_FAILURE );
			}
			
		}
		
}

#endif
