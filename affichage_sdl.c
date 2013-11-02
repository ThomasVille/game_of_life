#include "affichage_sdl.h"


/** Dessine un pixel
 * screen : Surface cible
 * x, y : Coordonnées du point
 * color : Couleur du pixel à dessiner
**/
void setPixel(SDL_Surface *screen, int x, int y, Uint32 color)
{
	if(x < 0 || y < 0 || x >= screen->w || y >= screen->h)
		return;
	
	*((Uint32*)(screen->pixels) + x + y * screen->w) = color;
}


void drawSquare(SDL_Surface *screen, int coteBloc, int hBlocs, int vBlocs)
{
	SDL_Rect blitrect = {hBlocs*coteBloc, vBlocs*coteBloc,0,0};
	SDL_Surface *carre = NULL;
	Uint32 color = SDL_MapRGB(screen->format, 255,255,255);
	// TODO Calculer width et length avant
	carre = SDL_CreateRGBSurface(0,coteBloc,coteBloc,32, 0, 0, 0,255);
	SDL_FillRect(carre, NULL, color);
	SDL_BlitSurface(carre,NULL, screen, &blitrect);
	
	SDL_FreeSurface(carre);
}

void drawSquareAbsolute(SDL_Surface *screen, int coteBloc, int x, int y)
{
	SDL_Rect blitrect = {x, y,0,0};
	SDL_Surface *carre = NULL;
	Uint32 color = SDL_MapRGB(screen->format, 255,255,255);
	// TODO Calculer width et length avant
	carre = SDL_CreateRGBSurface(0,coteBloc,coteBloc,32, 0, 0, 0,255);
	SDL_FillRect(carre, NULL, color);
	SDL_BlitSurface(carre,NULL, screen, &blitrect);
	
	SDL_FreeSurface(carre);
}

void drawGrid(SDL_Surface *screen, int coteBloc, int x, int y, int w, int h)
{
	int i, j;
	/// Parcours case par case puis pixel par pixel
	for(i = x; i < x + w*coteBloc+1; i += coteBloc)
	{
		for(j = y; j < y + h*coteBloc; j++)
		{
			setPixel(screen, i,j, SDL_MapRGB(screen->format, 100,100,100));
		}
	}
	
	for(j = y; j < y + h*coteBloc+1; j += coteBloc)
	{
		for(i = x; i < x + w*coteBloc; i++)
		{
			setPixel(screen, i,j, SDL_MapRGB(screen->format, 100,100,100));
		}
	}
}

/* Initialise la SDL
 * ptr_screen : Pointeur sur le pointeur de la SDL_Surface
 * ptr_police : Pointeur sur la police de caractère utilisée
 * On récupère un pointeur sur un pointeur pour modifier le pointeur
 * original
*/
void initSDL(SDL_Surface **ptr_screen, TTF_Font **ptr_police, int w, int h)
{
	/*************************** Initialisation SDL	***************************/ 
	atexit(SDL_Quit);
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Erreur à l'initialisation de la SDL : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
 
	// Modifie la valeur du pointeur 'screen'
	*ptr_screen = SDL_SetVideoMode(w, h, 32, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
	
	if (*ptr_screen == NULL) {
		fprintf(stderr, "Impossible d'activer le mode graphique : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	if(TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
	
	*ptr_police = TTF_OpenFont("Font.ttf", 30);
	SDL_WM_SetCaption("Conway's Game of Life !", NULL);
	

	/*************************** Initialisation SDL	***************************/ 
}

void drawGameMatrix(SDL_Surface *screen, Simulation *sim, int grid)
{
	int x = 0, y = 0, i, j;
	int xCoin = sim->dispX - 0.5*sim->largeurCase*sim->nbCasesX;
	int yCoin = sim->dispY - 0.5*sim->largeurCase*sim->nbCasesY;
	
	for(i = 0; i < sim->nbCasesX; i++)
	{
		for(j = 0; j < sim->nbCasesY; j++)
		{
			if(sim->buffers[sim->actualBuffer][i][j])
			{
				x = xCoin + i*sim->largeurCase;
				y = yCoin + j*sim->largeurCase;
				
				drawSquareAbsolute(screen, sim->largeurCase, x, y);
			}
		}
	}
	if(grid)
		drawGrid(screen, sim->largeurCase, xCoin, yCoin, sim->nbCasesX, sim->nbCasesY);
}



Fps initFps()
{
	Fps fps;
	fps.goal = 600;
	fps.ticksBetweenFrames = 1000/fps.goal;
	fps.nextTick = 0;
	fps.real = fps.goal;
	fps.realCpt = 0;
	fps.lastSecondTicks = 0;
	
	return fps;
}



void tempoFps(Fps *fps)
{
	// Si on doit attendre avant la prochaine frame
	if(fps->nextTick > SDL_GetTicks())
		SDL_Delay(fps->nextTick - SDL_GetTicks());
	// On calcule le tick où la prochaine frame va s'afficher
	fps->nextTick = SDL_GetTicks() + fps->ticksBetweenFrames;
}

void computeTrueFps(Fps *fps)
{
	if(SDL_GetTicks() >= fps->lastSecondTicks+1000)
	{
		fps->real = fps->realCpt;
		fps->realCpt = 0;
		fps->lastSecondTicks = SDL_GetTicks();
	}
}

