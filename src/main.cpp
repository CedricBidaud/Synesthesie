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
#include "Leapfrog.hpp"

#include "UniversalForce.hpp"

#include "SoundManager.hpp"
#include "Instrument.hpp"

#include "imgui.h"
#include "imguiRenderGL.h"

#include "utils.hpp"

using namespace imac3;


int main() {
	
    WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Synesthésie");

    wm.setFramerate(30);
    
    // ----
    // PARTICLES
    // ----
    
    // Création des particules
    ParticleManager particleManager;


	// ----
	// INSTRUMENTS
	// ----
	
	Instrument bass(glm::vec2(0.3f, 0.5f), glm::vec2(-0.04, 0.07), Instrument::bass, glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0, 1000);
	Instrument drums(glm::vec2(-0.3f, -0.5f), glm::vec2(-0.04, 0.07), Instrument::drums, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0, 5000);
	Instrument guitarA(glm::vec2(-0.5f, 0.3f), glm::vec2(-0.04, 0.07), Instrument::guitarA, glm::vec3(0.0f, 0.0f, 0.1f), glm::vec3(0.0f, 1.0f, 1.0f), 100, 2000);
	Instrument guitarB(glm::vec2(0.5f, -0.3f), glm::vec2(-0.04, 0.07), Instrument::guitarB, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), 100, 2000);
	
	std::vector<Instrument> band;
	band.push_back(bass);
	band.push_back(drums);
	band.push_back(guitarA);
	band.push_back(guitarB);

	// ----
	// SOUND
	// ----
	
	int spectrumSize=4096; // Higher means more frequency precision
	
	int calibrationDuration = 100;
	
	SoundManager soundManager;
	soundManager.Init(spectrumSize);
	float spectrum[spectrumSize];
	

	// ----
	// SHADERS
	// ----
	
	// Try to load and compile shader
    ShaderGLSL particleShader, polyShader, quadShader;
	loadShaders(&particleShader, &polyShader, &quadShader);

    ParticleRenderer2D renderer(&soundManager, particleShader.program, polyShader.program, quadShader.program, band);

   
    //~ particleManager.addRandomParticles(1);

	Leapfrog leapfrog;
    
    // UniversalForce			 (const Leapfrog& solver, float fKRep, float fKSticky, float fLInf, float fLSup, float fConstK, float fConstL, float dt, float brakeV, float brakeL){
    UniversalForce universalForce(leapfrog, 			  0.191f, 		0.0f, 			0.145, 		0.159, 		 0.2, 			0.2,			0.0f,	 0.011,			0.159);   
    

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
	
	int quadTriangleCount = 2;
	
	// Framebuffer
	GLuint framebuffer;
	GLuint texColorBuffer;
	
	GLuint vao;
	GLuint vbo[3];
	
	float blurSize = 4.0f;
	float particleSize = 1.0f;
	

	loadGeometry(&vao, vbo, &framebuffer, &texColorBuffer);
	
	// fin framebuffer
	
    while(!done) {
        wm.startMainLoop();

        // -------
        // - SON -
        // -------
        
        soundManager.GetSpectrum(spectrum);
        //std::cout << soundManager.GetRelMaxFrequency(spectrum, 0, 1500) << std::endl;
		//repulsiveForce.m_fLInf = 0.05+(spectrum[80]+spectrum[160]+spectrum[240]+spectrum[400]+spectrum[800]+spectrum[3200])*8;
		universalForce.m_fLInf = 0.01+0.2*soundManager.GetVolume(spectrum);
		//std::cout << repulsiveForce.m_fLInf << std::endl;
		
		if(soundManager.getCalibrationDuration() != 0){
			soundManager.calibrateVolume(spectrum);
		}
		
		soundManager.Update();
        
        
        // Rendu
        renderer.clear();
        
        if(bass.open){
			//~ bass.addParticle();addRandomParticle(m_position, m_velocity, m_lowColor, m_index);
			particleManager.addRandomParticle(bass.m_position, bass.m_velocity, bass.m_lowColor, bass.m_index);
		}
		if(drums.open){
			//~ drums.addParticle();
			particleManager.addRandomParticle(drums.m_position, drums.m_velocity, drums.m_lowColor, drums.m_index);
		}
		if(guitarA.open){
			//~ guitarA.addParticle();
			particleManager.addRandomParticle(guitarA.m_position, guitarA.m_velocity, guitarA.m_lowColor, guitarA.m_index);
		}
		if(guitarB.open){
			//~ guitarB.addParticle();
			particleManager.addRandomParticle(guitarB.m_position, guitarB.m_velocity, guitarB.m_lowColor, guitarB.m_index);
		}
		
		glViewport(0,0,TEXTURE_WIDTH,TEXTURE_HEIGHT);
		// Écriture de l'image dans le framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glClear(GL_COLOR_BUFFER_BIT);
			particleManager.drawParticles(renderer, particleSize, soundManager.GetVolume(spectrum));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
		// Affichage de la texture (passage dans le shader pour blur et seuillage)
		renderer.drawQuad(vao, texColorBuffer, quadTriangleCount, blurSize, 0);
		
		
		// Simulation
		universalForce.setDt(dt);
        universalForce.apply(particleManager);

        
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
					
					float actualInf = universalForce.m_fLInf;
					float actualSup = universalForce.m_fLSup;
					
					imguiSlider("Repulsive KRep", &universalForce.m_fKRep, 0.f, 2.f, 0.001f);
					imguiSlider("Repulsive KSticky", &universalForce.m_fKSticky, 0.f, 2.f, 0.001f);
					imguiSlider("Repulsive LInf", &universalForce.m_fLInf, 0.f, 0.5f, 0.001f);
					imguiSlider("Repulsive LSup", &universalForce.m_fLSup, 0.f, 0.5f, 0.001f);
					imguiSlider("Constant L", &universalForce.m_fConstL, 0.f, 2.f, 0.001f);
					
					float dInf = universalForce.m_fLInf - actualInf;
					float dSup = universalForce.m_fLSup - actualSup;
					
					if(link == true){
						universalForce.m_fLInf += dSup;
						universalForce.m_fLSup += dInf;
					}
				}
				
				if(brakeIHM == false){
					imguiLabel("Brake Force");
					
					imguiSlider("Brake V", &universalForce.m_fBrakeV, 0.f, 1.f, 0.001f);
					imguiSlider("Brake L", &universalForce.m_fBrakeL, 0.f, 0.8f, 0.001f);
					imguiSlider("Brake Amort", &universalForce.m_fBrakeAmort, 0.0f, 0.01f, 0.0001f);
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
					
				case SDL_KEYUP:
					switch(e.key.keysym.sym){
							
						case SDLK_SPACE:
							open = 0;
							break;	
							
						// Instruments
						case SDLK_UP:
							drums.open = 0;
							break;
						
						case SDLK_DOWN:
							bass.open = 0;
							break;
							
						case SDLK_LEFT:
							guitarA.open = 0;
							break;
							
						case SDLK_RIGHT:
							guitarB.open = 0;
							break;
					}
					break;	
					
					
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym){
						case SDLK_ESCAPE:
							done = true;
							break;
							
						case SDLK_v:
							soundManager.setCalibrationDuration(calibrationDuration);
							break;
							
						case SDLK_SPACE:
							open = 1;
							break;
							
						// Instruments
						case SDLK_UP:
							drums.open = 1;
							break;
						
						case SDLK_DOWN:
							bass.open = 1;
							break;
							
						case SDLK_LEFT:
							guitarA.open = 1;
							break;
							
						case SDLK_RIGHT:
							guitarB.open = 1;
							break;
							
						case SDLK_c:
							particleManager.clean();
							break;
							
						//~ case SDLK_k:
							//~ std::cout << "Scaled volume : " << soundManager.scaleVolume(soundManager.GetVolume(spectrum)) << std::endl;
							//~ break;
							
						case SDLK_s:
							if (!config.is_open())
							{
								config.open ("config.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
								if (config.is_open())
								{
									config << universalForce.m_fConstL << "\n";
									config << universalForce.m_fKRep << "\n";
									config << universalForce.m_fKSticky << "\n";
									config << universalForce.m_fLInf << "\n";
									config << universalForce.m_fLSup << "\n";
									config << universalForce.m_fBrakeV << "\n";
									config << universalForce.m_fBrakeL << "\n";
									config << universalForce.m_fBrakeAmort << "\n";
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
								universalForce.m_fConstL = f;
								getline (config,value);		
								std::istringstream(value) >> f; 
								universalForce.m_fKRep = f;
								getline (config,value);
								std::istringstream(value) >> f; 
								universalForce.m_fKSticky = f;
								getline (config,value);
								std::istringstream(value) >> f; 
								universalForce.m_fLInf = f;
								getline (config,value);
								std::istringstream(value) >> f; 
								universalForce.m_fLSup = f;
								getline (config,value);
								std::istringstream(value) >> f; 
								universalForce.m_fBrakeV = f;
								getline (config,value);
								std::istringstream(value) >> f; 
								universalForce.m_fBrakeL = f;
								getline (config,value);
								std::istringstream(value) >> f; 
								universalForce.m_fBrakeAmort = f;
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

