#pragma once
#include "Renderer.h"
#include "Game.h"

#define MAIN_MENU 0
#define HISTORIC_MENU 1
#define GAME_MENU 2
#define PAUSE_MENU 3


typedef struct Interface2d
{
	Window* window;
	Camera* camera;
	Game* game;

	Renderer2d r2d;
	Renderer renderer;

	Object2dData mainMenu;

	//Object2dData affichageActualScore;
	
	int currentMenu;

	char buffer[200];
	int windowHeigt;
	int windowWidth;

	int actualScore;

}Interface2d;


void interface2dCreate(Interface2d* self, Window* window, Camera* camera);
void interface2dObjectAdd(Interface2d* self);

void interface2dFrame(Interface2d* self);

void interface2dMainMenu(Interface2d* self);
void interface2dHistoricMenu(Interface2d* self);
void interface2dGameMenu(Interface2d* self);
void interface2dPauseMenu(Interface2d* self);


void Renderer2dHistory(Interface2d* self);
void Rendere2dClearHistory(Interface2d* self);

