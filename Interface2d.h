#pragma once
#include "Renderer.h"


typedef struct Interface2d
{
	Window* window;
	Camera* camera;

	Renderer2d r2d;

	Object2dData buttonStart;
	Object2dData buttonClose;

	Object2dData buttonHistory;
	Object2dData buttonClear;

	Object2dData buttonBack;

	Object2dData mainMenu;

	Object2dData actualScore;

	int isMenuOn;

}Interface2d;

void interface2dCreate(Interface2d* self, Window* window, Camera* camera);
void interface2dObjectAdd(Interface2d* self);

void interface2dFrame(Interface2d* self);

void interface2dMenuOn(Interface2d* self);
void interface2dMenuOFF(Interface2d* self);

void Renderer2dHistory(Interface2d* self);
void Rendere2dClearHistory(Interface2d* self);

