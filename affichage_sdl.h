#ifndef _H_AFFICHAGE_SDL_
#define _H_AFFICHAGE_SDL_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "simulation.h"

struct Fps
{
	/// Nombre de FPS qu'on veut atteindre
	int goal;
	/// Temps qui se passe entre deux frames (en millisecondes)
	int ticksBetweenFrames;
	/// Tick à attendre avant la prochaine frame (en millisecondes)
	int nextTick;
	/// Vrais FPS
	int real, realCpt, lastSecondTicks;
};
typedef struct Fps Fps;

/// Dessine un pixel
void setPixel(SDL_Surface *screen, int x, int y, Uint32 color);
void drawSquare(SDL_Surface *screen, int coteBloc, int hBlocs, int vBlocs);
/** Dessine une grille de coin supérieur gauche (x,y) de pas 'coteBloc' et de nombre de cases (w,h) **/
void drawGrid(SDL_Surface *screen, int coteBloc, int x, int y, int w, int h);
/** Initialise la SDL **/
void initSDL(SDL_Surface **ptr_screen, TTF_Font **ptr_police, int w, int h);

void drawGameMatrix(SDL_Surface *screen, Simulation *sim, int grid);
Fps initFps();
void tempoFps(Fps *fps);
void computeTrueFps(Fps *fps);

#endif
