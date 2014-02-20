#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream> 

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "renderer/WindowManager.hpp"
#include "renderer/ParticleRenderer2D.hpp"

#include <vector>

#include "ParticleManager.hpp"
#include "ConstantForce.hpp"
#include "Leapfrog.hpp"

#include "HookForce.hpp"
#include "RepulsiveForce.hpp"
#include "StickyForce.hpp"
#include "BrakeForce.hpp"

#include "SoundManager.hpp"

#include "imgui.h"
#include "imguiRenderGL.h"

static const Uint32 WINDOW_WIDTH = 1024;
static const Uint32 WINDOW_HEIGHT = 1024;

using namespace imac3;

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

int compile_and_link_shader(ShaderGLSL & shader, int typeMask, const char * sourceBuffer, int bufferSize);
int load_shader_from_file(ShaderGLSL & shader, const char * path, int typemask);
int destroy_shader(ShaderGLSL & shader);

int main() {
	
    WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Synesthesie");
    wm.setFramerate(30);

	// ----
	// SOUND
	// ----
	
	SoundManager soundManager;
	soundManager.Init();
	float spectrum[4096];

	

	// ----
	// SHADERS
	// ----
	
	// Try to load and compile shader
    ShaderGLSL particleShader;
    const char * shaderFile = "../src/shaders/particle.glsl";
    //~ int status = load_shader_from_file(shader, shaderFile, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER | ShaderGLSL::GEOMETRY_SHADER);
    int status = load_shader_from_file(particleShader, shaderFile, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER);
    if ( status == -1 )
    {
        fprintf(stderr, "Error on loading  %s\n", shaderFile);
        exit( EXIT_FAILURE );
    }

   
	ShaderGLSL polyShader;
    shaderFile = "../src/shaders/poly.glsl";
    //~ int status = load_shader_from_file(shader, shaderFile, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER | ShaderGLSL::GEOMETRY_SHADER);
    status = load_shader_from_file(polyShader, shaderFile, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER);
    if ( status == -1 )
    {
        fprintf(stderr, "Error on loading  %s\n", shaderFile);
        exit( EXIT_FAILURE );
    }
    
    
    ShaderGLSL quadShader;
    shaderFile = "../src/shaders/quad.glsl";
    //~ int status = load_shader_from_file(shader, shaderFile, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER | ShaderGLSL::GEOMETRY_SHADER);
    status = load_shader_from_file(quadShader, shaderFile, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER);
    if ( status == -1 )
    {
        fprintf(stderr, "Error on loading  %s\n", shaderFile);
        exit( EXIT_FAILURE );
    }
    

    ParticleRenderer2D renderer(particleShader.program, polyShader.program, quadShader.program);

    // Création des particules
    ParticleManager particleManager;
    particleManager.addRandomParticles(1);

	Leapfrog leapfrog;

	ConstantForce constForce(0.2,0.2);

    // hookForce(K,L)
    float L = 0.05f;
    HookForce hookForce(0.2f, 0.15f);
    //~ RepulsiveForce(float fKRep, float fKSticky, float fLInf, float fLSup);
    RepulsiveForce repulsiveForce(0.191f, 0.282f, 0.145, 0.159);
    
    //~ BrakeForce(float dt, float v, float l, const Leapfrog& solver);
    BrakeForce brakeForce(0.0f, 0.011, 0.159, leapfrog);
	
	
	// ----- Boxes -----
	//~ static Polygon 		buildBox(glm::vec3 color, 		 glm::vec2 position,  float width, float height, float rotationInDegrees);
	//~ Polygon box1 = Polygon::buildBox(glm::vec3(0.9,0.8,0.3), glm::vec2(-0.38,-0.2), 0.25, 		   0.12, 		 -78.0f);
	//~ Polygon box2 = Polygon::buildBox(glm::vec3(0.2,0.2,0.9), glm::vec2(0.1,0.5), 0.05, 		   0.47, 		 30.0f);
    //~ 
    //~ Polygon boxL = Polygon::buildBox(glm::vec3(1.0), glm::vec2(-1.0,0.0),0.1,2.0,false);
    //~ Polygon boxR = Polygon::buildBox(glm::vec3(1.0), glm::vec2(1.0,0.0),0.1,2.0,false);
    //~ Polygon boxT = Polygon::buildBox(glm::vec3(1.0), glm::vec2(0.0,1.0),2.0,0.1,false);
    //~ Polygon boxB = Polygon::buildBox(glm::vec3(1.0), glm::vec2(0.0,-1.0),2.0,0.1,false);
    
    
    // ----- Forces de répulsion des boxes -----
    float polyStickyCoef = 1.005f;
	
	//~ PolygonForce box1PolyForce(box1, polyStickyCoef, leapfrog);
	//~ PolygonForce box2PolyForce(box2, polyStickyCoef, leapfrog);
	//~ 
	//~ PolygonForce boxLPolyForce(boxL, polyStickyCoef, leapfrog);
	//~ PolygonForce boxRPolyForce(boxR, polyStickyCoef, leapfrog);
	//~ PolygonForce boxTPolyForce(boxT, polyStickyCoef, leapfrog);
	//~ PolygonForce boxBPolyForce(boxB, polyStickyCoef, leapfrog);
	
	// ----- Ajout des boxes et de leurs forces à un vecteur -----
	// => rendu et application des forces dans la même passe
	//~ std::vector<std::pair<Polygon, PolygonForce>> polysAndForces;
    //~ polysAndForces.push_back(std::pair<Polygon, PolygonForce>(box1, box1PolyForce));
    //~ polysAndForces.push_back(std::pair<Polygon, PolygonForce>(box2, box2PolyForce));
    //~ polysAndForces.push_back(std::pair<Polygon, PolygonForce>(boxL, boxLPolyForce));
    //~ polysAndForces.push_back(std::pair<Polygon, PolygonForce>(boxR, boxRPolyForce));
    //~ polysAndForces.push_back(std::pair<Polygon, PolygonForce>(boxT, boxTPolyForce));
    //~ polysAndForces.push_back(std::pair<Polygon, PolygonForce>(boxB, boxBPolyForce));
    
    //
    glm::vec2 click;
    
	
    // Temps s'écoulant entre chaque frame
    float dt = 0.f;
	float pas = 0.001;
	bool open = false;
	bool done = false;
	
	
	/*
	 * 
	 * IHM
	 * 
	 */
	
	GLenum error;
	if(GLEW_OK != (error = glewInit())) {
			std::cerr << "Impossible d'initialiser GLEW: " << glewGetErrorString(error) << std::endl;
			return EXIT_FAILURE;
	}
	
	bool ihm = true;
	bool ihmForTom = false;
    
    bool is_lClicPressed = false;
    
    int uiWidth = 300;
	int uiHeight = 500;
	int detailsUIHeight = 170;
	int camUIHeight = 200;
	int viewUIHeight = 250;
	
	int mousex = 0;
	int mousey = 0;
	int uiScrollTest = 0;
	int detailsUIscrollArea = 0;
	int camUIscrollArea = 0;
	int viewUIscrollArea = 0;
	int closeScrollArea = 0;
	
	int toggle = 0;
	
	bool repulsiveIHM = true;
	bool brakeIHM = true;
	bool postIHM = true;
	
	bool link = false;
	
	
	if (!imguiRenderGLInit("DroidSans.ttf")){
		fprintf(stderr, "Could not init GUI renderer.\n");
		exit(EXIT_FAILURE);
	}

	
	// -----
	// Post-traitement
	// -----

	
	// Framebuffer
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	
	// Texture
	GLuint texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	
	
	// VAO du quad d'affichage
	int quadTriangleCount = 2;
	int quadTriangleList[] = {0,1,2, 2,1,3};
	float quadVertices[] = 
		{
			-1.f,	1.f,
			-1.f,	-1.f,
			1.f,	1.f,
			1.f,	-1.f
		};
	float quadUVs[] = 
		{
			0.f,	1.f,
			0.f,	0.f,
			1.f,	1.f,
			1.f,	0.f
		};
		
	float blurSize = 4.0f;
	float particleSize = 1.0f;
	
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	
	GLuint vbo[3];
	glGenBuffers(3, vbo);
	
	// renseignement du vao
	glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadTriangleList), quadTriangleList, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*2, (void*)0);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
			
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*2, (void *)0);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadUVs), quadUVs, GL_STATIC_DRAW);
			
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	// link du framebuffer et de la texture
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
			);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0
			);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		GLenum compStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		
		if(compStatus == GL_FRAMEBUFFER_COMPLETE){
			std::cout << "Framebuffer complet ! Code status : " << compStatus << std::endl;
		}else{
			std::cout << "Erreur : framebuffer incomplet - code status : " << compStatus << std::endl;
		}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	
    while(!done) {
        wm.startMainLoop();

        // Rendu
        renderer.clear();
        
        soundManager.GetSpectrum(spectrum);
        //std::cout << soundManager.GetRelMaxFrequency(spectrum, 0, 1500) << std::endl;
		repulsiveForce.m_fLInf = 0.05+(spectrum[80]+spectrum[160]+spectrum[240]+spectrum[400]+spectrum[800]+spectrum[3200])*8;
		//std::cout << repulsiveForce.m_fLInf << std::endl;
		soundManager.Update();
        
        if(open){
			particleManager.addRandomParticles(1);
		}
		
		// Écriture de l'image dans le framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glClear(GL_COLOR_BUFFER_BIT);
			particleManager.drawParticles(renderer, particleSize);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		
		// Affichage de la texture (passage dans le shader pour blur et seuillage)
		renderer.drawQuad(vao, texColorBuffer, quadTriangleCount, blurSize, 0);
		
		
		// Simulation
        constForce.apply(particleManager);

        repulsiveForce.apply(particleManager);
        
        brakeForce.setDt(dt);
        brakeForce.apply(particleManager);

		//~ for(int i = 0; i < polysAndForces.size(); ++i){
			//~ polysAndForces[i].first.draw(renderer);
			//~ polysAndForces[i].second.setDt(dt);
			//~ polysAndForces[i].second.apply(particleManager);
		//~ }
        
        leapfrog.solve(particleManager, dt);
        
        
        // --- IMGUI ---
        
        if (ihmForTom==true){
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_DEPTH_TEST);
			
			SDL_GetMouseState(&mousex, &mousey);
			
			mousey = WINDOW_HEIGHT - mousey;
			imguiBeginFrame(mousex, mousey, is_lClicPressed, 0);        
			
			imguiBeginScrollArea("Forces", WINDOW_WIDTH - uiWidth, WINDOW_HEIGHT - (uiHeight+10), uiWidth, uiHeight, &uiScrollTest);
			imguiSeparatorLine();
			imguiSeparator();
			
			if(imguiButton("Afficher Interface")){
				ihm = !ihm;
			}
			
			if(ihm == true) {
				char lineBuffer[512];
				sprintf(lineBuffer, "Nb Particules %d", particleManager.getSize());
				imguiLabel(lineBuffer);
				imguiSeparator();
				
				if(imguiItem("Repulsive Force", repulsiveIHM)){
					repulsiveIHM = false;
					brakeIHM = true;
					postIHM = true;
				}
				
				if(imguiItem("Brake Force", brakeIHM)){
					brakeIHM = false;
					repulsiveIHM = true;
					postIHM = true;
				}
				
				if(imguiItem("Shading", postIHM)){
					postIHM = false;
					repulsiveIHM = true;
					brakeIHM = true;
				}
				
				imguiSeparatorLine();
				
				
			
				if(repulsiveIHM == false){
					imguiLabel("Repulsive Force");
					
					
					if(imguiCheck("Lier les bornes", link, true)){
						link = !link;
					}
					
					float actualInf = repulsiveForce.m_fLInf;
					float actualSup = repulsiveForce.m_fLSup;
					
					imguiSlider("Repulsive KRep", &repulsiveForce.m_fKRep, 0.f, 2.f, 0.001f);
					imguiSlider("Repulsive KSticky", &repulsiveForce.m_fKSticky, 0.f, 2.f, 0.001f);
					imguiSlider("Repulsive LInf", &repulsiveForce.m_fLInf, 0.f, 0.5f, 0.001f);
					imguiSlider("Repulsive LSup", &repulsiveForce.m_fLSup, 0.f, 0.5f, 0.001f);
					
					float dInf = repulsiveForce.m_fLInf - actualInf;
					float dSup = repulsiveForce.m_fLSup - actualSup;
					
					if(link == true){
						repulsiveForce.m_fLInf += dSup;
						repulsiveForce.m_fLSup += dInf;
					}
				}
				
				if(brakeIHM == false){
					imguiLabel("Brake Force");
					
					imguiSlider("Brake V", &brakeForce.m_fV, 0.f, 1.f, 0.001f);
					imguiSlider("Brake L", &brakeForce.m_fL, 0.f, 0.8f, 0.001f);
					imguiSlider("Brake Amort", &brakeForce.m_fAmort, 0.0f, 0.01f, 0.0001f);
				}
				
				if(postIHM == false){
					imguiLabel("Shading");
					
					imguiSlider("Blur size", &blurSize, 1.f, 20.f, 0.5f);
					imguiSlider("Particle size", &particleSize, 0.1f, 3.0f, 0.01f);
				}
				
				imguiSeparator();
				if(imguiButton("Ouvrir / Fermer la vanne (Spacebar)")){
					open = !open;
				}
				
				imguiSeparator();
				if(imguiButton("Reset ('c')")){
					particleManager.clean();
				}
				
			}
			
			imguiEndScrollArea();
			imguiEndFrame();
			
			imguiRenderGLDraw(WINDOW_WIDTH, WINDOW_HEIGHT);
			
			// Mise à jour de l'affichage
			SDL_GL_SwapBuffers();
							
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);

		}
		
		// Sauvegarde de la config
		std::fstream config;
		
        // Gestion des evenements
		SDL_Event e;
        while(wm.pollEvent(e)) {
			switch(e.type) {
				default:
					break;
					
				case SDL_MOUSEBUTTONDOWN:
					switch(e.button.button){
						case SDL_BUTTON_LEFT:
							is_lClicPressed = true;
							click.x = (float(e.button.x) / WINDOW_WIDTH - 0.5f)*2.0;
							click.y = (float(e.button.y) / WINDOW_HEIGHT - 0.5f)*(-2.0);
							std::cout << "click : " << click.x << " - " << click.y << std::endl;
							//~ for(int i = 0; i < polysAndForces.size(); ++i){
								//~ Polygon poly = polysAndForces[i].first;
								//~ 
								//~ float angle = poly.rotationInDegrees;
								//~ 
								//~ poly.rotate(-angle);
								//~ 
								//~ /*
								//~ glm::vec2 tempPoint;
								//~ 
								//~ 
								//~ for(int i = 0; i < m_pointsArray.size(); ++i){
									//~ m_pointsArray[i] = glm::vec2(m_pointsArray[i].x*_cos - m_pointsArray[i].y*_sin, m_pointsArray[i].x*_sin+m_pointsArray[i].y*_cos);
								//~ }
								//~ */
								//~ 
								//~ float _cos = cos(-3.1416*angle/180.0);
								//~ float _sin = sin(-3.1416*angle/180.0);
								//~ 
								//~ glm::vec2 tempClick = glm::vec2(click.x*_cos - click.y*_sin, click.x*_sin+click.y*_cos);
								//~ 
								//~ 
								//~ float minX = 1000.0;
								//~ float maxX = -1000.0;
								//~ float minY = 1000.0;
								//~ float maxY = -1000.0;
								//~ 
								//~ for(int j = 0; j < poly.getSize(); ++j){
									//~ glm::vec2 p = poly.m_pointsArray[j];
									//~ (p.x < minX) ? minX = p.x : minX;
									//~ (p.x > maxX) ? maxX = p.x : maxX;
									//~ (p.y < minY) ? minY = p.y : minY;
									//~ (p.y > maxY) ? maxY = p.y : maxY;
								//~ }
								//~ 
								//~ std::cout << "minX : " << minX << ", minY : " << minY << ", maxX : " << maxX << ", maxY : " << maxY << std::endl;
								//~ 
								//~ if(tempClick.x > minX && tempClick.x < maxX && tempClick.y > minY && tempClick.y < maxY){
									//~ std::cout << "In !" << std::endl;
									//~ polysAndForces.erase(polysAndForces.begin()+i);
								//~ }else{
									//~ std::cout << "Out !" << std::endl;
								//~ }
								//~ 
								//~ std::cout << "" << std::endl;
								//~ 
							//~ }
							break;
							
						default:
							break;
					}
					break;
									
									
												
				case SDL_MOUSEBUTTONUP:
					switch(e.button.button){
						case SDL_BUTTON_LEFT:
							is_lClicPressed = false;
							break;
						
						default:
							break;
					}
					break;
							
					
					
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym){
						case SDLK_ESCAPE:
							done = true;
							break;
							
						case SDLK_SPACE:
							open = !open;
							break;
							
						case SDLK_c:
							particleManager.clean();
							break;
							
						case SDLK_s:
							if (!config.is_open())
							{
								config.open ("config.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
								if (config.is_open())
								{
									config << constForce.m_force.y << "\n";
									config << repulsiveForce.m_fKRep << "\n";
									config << repulsiveForce.m_fKSticky << "\n";
									config << repulsiveForce.m_fLInf << "\n";
									config << repulsiveForce.m_fLSup << "\n";
									config << brakeForce.m_fV << "\n";
									config << brakeForce.m_fL << "\n";
									config << brakeForce.m_fAmort << "\n";
									std::cout << "Config written \n";
								} else {
									std::cout << "Unable to open config \n";
								}
								config.close();
							}
							break;
							
						case SDLK_l:
							std::string value;
							float f;
							config.open ("config.txt");
							if (config.is_open())
							{
								std::cout << "Config is open \n";
								getline (config,value);		
								std::istringstream(value) >> f; 
								constForce.m_force.y = f;
								getline (config,value);		
								std::istringstream(value) >> f; 
								repulsiveForce.m_fKRep = f;
								getline (config,value);
								std::istringstream(value) >> f; 
								repulsiveForce.m_fKSticky = f;
								getline (config,value);
								std::istringstream(value) >> f; 
								repulsiveForce.m_fLInf = f;
								getline (config,value);
								std::istringstream(value) >> f; 
								repulsiveForce.m_fLSup = f;
								getline (config,value);
								std::istringstream(value) >> f; 
								brakeForce.m_fV = f;
								getline (config,value);
								std::istringstream(value) >> f; 
								brakeForce.m_fL = f;
								getline (config,value);
								std::istringstream(value) >> f; 
								brakeForce.m_fAmort = f;
								std::cout << "Config loaded \n";
								config.close();
							}
							else std::cout << "Unable to open config \n";
							break;
							
					}
					break;
					
				
				case SDL_QUIT:
					done = true;
					break;
			}
		}
        // Mise à jour de la fenêtre
        dt = wm.update();
	}
	
	// Framebuffer
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteTextures(1, &texColorBuffer);
	
	
	// imgui
    imguiRenderGLDestroy();

	return EXIT_SUCCESS;
}


// -------------------
// 
// ----- SHADERS -----
//
// -------------------

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
