#ifndef _H_SIMULATION_
#define _H_SIMULATION_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

struct Simulation
{
	char ***buffers;
	int actualBuffer;
	int nbCasesX, nbCasesY, pause, largeurCase;
	// Position de la vue
	int dispX, dispY;
};
typedef struct Simulation Simulation;

int initSimulation(Simulation *sim, int nbCasesX, int nbCasesY);
void initGameSurface(Simulation *sim, int type);
void computeFrame(Simulation *sim);
int countNeighbours(Simulation *sim, int x, int y);

/// Crée un tableau de char de dimensions (d1,d2,d3)
char ***create3DTab(int d1, int d2, int d3);
/// Libère un tableau de char de premières dimensions (d1,d2)
void free3DTab(char ****tab, int d1, int d2);


#endif
