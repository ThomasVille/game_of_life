/*******************************************
 * Fichier	: vie_sdl.c
 * Auteur	: VILLE Thomas
 * Date		: 26/10/2013
*******************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <time.h>
#include <pthread.h>

#include "affichage_sdl.h"
#include "simulation.h"


int main(int argc, char **argv)
{
	// TEST LES ARGUMENTS //
	if(argc < 5)
	{
		printf("Usage : vie_sdl width height nbCasesX nbCasesY\nExemple : ./vie_sdl 1920 1080 190 100\n");
		return -1;
	}
	printf("Dimensions de la fenetre : %dx%d\nDimensions de la surface de jeu : %dx%d\n", atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
	
	// VARIABLES //
	/// Dimensions de la fenetre
	int winWidth = atoi(argv[1]), winHeight = atoi(argv[2]);
	
	SDL_Surface *screen = NULL, *text = NULL;
	TTF_Font *police = NULL;
	SDL_Event event;
	SDL_Color couleurBlanche = {255, 255, 255};
	
	/// Titre de la fenêtre, infos à afficher
	char caption[64], infos[200];
	int continuer = 1, moving = 0, proliferation = 0, grid = 1;
	
	Fps fps = initFps();
	Simulation sim;
	
	// INITIALISATION //
	srand(time(NULL));
	/// Les arguments 3 et 4 sont les dimensions de la surface de jeu
	initSimulation(&sim, atoi(argv[3]), atoi(argv[4]));
	initSDL(&screen, &police, winWidth, winHeight);
	
	// BOUCLE PRINCIPALE //
	do
	{
		fps.realCpt++;
		/* Gère les évenements */
		while(SDL_PollEvent(&event))
		{
			
			switch(event.type)
			{
				// Clique droit = bouger la vue
				case SDL_MOUSEBUTTONDOWN:
					switch(event.button.button)
					{
						case SDL_BUTTON_RIGHT:
							moving = 1;
							break;
						default:
							break;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					switch(event.button.button)
					{
						case SDL_BUTTON_RIGHT:
							moving = 0;
							break;
						default:
							break;
					}
					break;
				// Si la souris bouge et que le clique droit est enfoncé
				// On déplace la vue
				case SDL_MOUSEMOTION:
					if(moving)
					{
						sim.dispX += event.motion.xrel;
						sim.dispY += event.motion.yrel;
					}
					break;
				case SDL_KEYDOWN: // Gère l'appui sur une touche
					switch(event.key.keysym.sym)
					{
						// Bouge la surface
						case SDLK_a:
							moving = 1;
							break;
						case SDLK_g:
							grid = !grid;
							break;
						// Active la fonction ANTICONVERGENCE !!
						case SDLK_UP:
							proliferation = !proliferation;
							break;
						// Met en pause ou reprend la simulation
						case SDLK_SPACE:
							sim.pause = !sim.pause;
							break;
						// Zoom + et -
						case SDLK_KP_PLUS:
							sim.largeurCase++;
							break;
						case SDLK_KP_MINUS:
							if(sim.largeurCase > 1)
								sim.largeurCase--;
							break;
						// Mode plein écran
						case SDLK_F11:
							screen = SDL_SetVideoMode(winWidth, winHeight, 32, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
							break;
						case SDLK_ESCAPE:
							continuer = 0;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym)
					{
						case SDLK_a:
							moving = 0;
							break;
						default:
							break;
					}
					break;
				// Redimensionnement de la fenêtre
				case SDL_VIDEORESIZE:
					winWidth = event.resize.w;
					winHeight = event.resize.h;
					screen = SDL_SetVideoMode(winWidth, winHeight, 32, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
					break;
				case SDL_QUIT:
					continuer = 0;
			}
		}
		
		/* Calculs */
		if(!sim.pause)
		{
			computeFrame(&sim);
			/// ANTICONVERGENCE
			if(proliferation)
				sim.buffers[sim.actualBuffer][rand()%sim.nbCasesX][rand()%sim.nbCasesY] = 1;
		}
		
		/* Dessine l'écran */
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 50,50,50));
		/// Dessine les carrés et la grille sur la surface du jeu
		drawGameMatrix(screen, &sim, grid);
			
		
		/// Ecrit les infos
		sprintf(infos, "dispX = %d dispY = %d", sim.dispX, sim.dispY);
		text = TTF_RenderText_Blended(police, infos, couleurBlanche);
		SDL_BlitSurface(text,NULL, screen, NULL);


		
		// Change le buffer de travail
		if(!sim.pause)
			sim.actualBuffer = !sim.actualBuffer;

		// Régule les FPS
		tempoFps(&fps);
		// Calcul les vrais FPS
		computeTrueFps(&fps);
		
		// Met à jour le titre de la fenêtre
		sprintf(caption, "Game Of Life ! %d fps !", fps.real);
		SDL_WM_SetCaption(caption, NULL);

		
		SDL_Flip(screen);
	}while (continuer);
	
	
	free3DTab(&sim.buffers, 2, sim.nbCasesX);
	
	TTF_CloseFont(police);
	TTF_Quit();
	
	SDL_FreeSurface(screen);
	SDL_Quit();
	return 0;
}
	
