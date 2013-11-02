#include "simulation.h"

/** Initialise la structure Simulation avec des valeurs par défaut **/
int initSimulation(Simulation *sim, int nbCasesX, int nbCasesY)
{
	sim->actualBuffer = 0;
	sim->nbCasesX = nbCasesX;
	sim->nbCasesY = nbCasesY;
	sim->pause = 0;
	sim->largeurCase = 10;
	sim->nbThread = 8;
	
	sim->dispX = (sim->nbCasesX*sim->largeurCase)/2;
	sim->dispY = (sim->nbCasesY*sim->largeurCase)/2;

	sim->buffers = create3DTab(2, sim->nbCasesX, sim->nbCasesY);
	
	initGameSurface(sim, 0);
	
	return 0;
}

/** Initialise la surface du jeu selon un schéma prédéterminé **/
void initGameSurface(Simulation *sim, int type)
{
	int x = 0, y = 0;
	
	for(x = 0; x < sim->nbCasesX; x++)
		for(y = 0; y < sim->nbCasesY; y++)
		{
			sim->buffers[!sim->actualBuffer][x][y] = rand()%2;
			sim->buffers[sim->actualBuffer][x][y] = 0;
		}
}

/** Calcul une nouvelle frame de la simulation sur un nouveau buffer **/
void computeFrame(Simulation *sim)
{
	int x = 0, y = 0, neighbours = 0, buf = 0;
	
	buf = sim->actualBuffer;
	
	for(x = 0; x < sim->nbCasesX; x++)
		for(y = 0; y < sim->nbCasesY; y++)
		{
			neighbours = countNeighbours(sim, x, y);
			// Une cellule vie dans tous les cas si elle a 3 voisines
			// vivantes ou reste en vie si elle a 2 voisines vivantes 
			if(neighbours == 3 || (sim->buffers[!buf][x][y] == 1 && neighbours == 2))
				sim->buffers[buf][x][y] = 1;
			else
				sim->buffers[buf][x][y] = 0;
		}
}

int countNeighbours(Simulation *sim, int x, int y)
{
	int neighbours = 0, i = 0, j = 0, buf = sim->actualBuffer;
	
	/* Compte toutes les cellules vivantes dans un carré de 3 autour de
	   la position (x,y) */
	for(i = x-1; i <= x+1; i++)
	{
		for(j = y-1; j <= y+1; j++)
		{
			if(i >= 0 && j >= 0 && i < sim->nbCasesX && j < sim->nbCasesY)
				neighbours += sim->buffers[!buf][i][j];
		}
	}
	// Enlève la cellule (x,y) du décompte
	neighbours -= sim->buffers[!buf][x][y];
	
	return neighbours;
}

char ***create3DTab(int d1, int d2, int d3)
{
	int i = 0;

	char ***tab = (char ***)malloc(d1*sizeof(char **));
	tab[0] = (char **)malloc(d2*sizeof(char*));
	tab[1] = (char **)malloc(d2*sizeof(char*));
	
	for(i = 0; i < d2; i++)
	{
		tab[0][i] = (char *)malloc(d3*sizeof(char));
		tab[1][i] = (char *)malloc(d3*sizeof(char));
	}
	
	return tab;
}

void free3DTab(char ****tab, int d1, int d2)
{
	int i = 0, j = 0;
	for(i = 0; i < d1; i++)
	{
		for(j = 0; j < d2; j++)
		{
			free((*tab)[i][j]);
		}
		free((*tab)[i]);
	}
	free(*tab);
}
