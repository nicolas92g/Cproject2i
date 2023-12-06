#pragma once
#include "Renderer.h"


typedef struct Interface2d
{
	Window* window;
	Camera* camera;

	Renderer2d r2d;

	Object2dData buttonStart;
	Object2dData buttonClose;
	Object2dData mainMenu;

	int isMenuOn;

}Interface2d;

void interface2dCreate(Interface2d* self, Window* window);
void interface2dObjectAdd(Interface2d* self);

void interface2dFrame(Interface2d* self, Camera* camera);
void interface2dMenuOn(Interface2d* self);
void interface2dMenuOFF(Interface2d);

