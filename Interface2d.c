#include "Interface2d.h"


void interface2dCreate(Interface2d* self, Window* window, Camera* camera)
{
	self->camera = camera;
	self->window = window;
	
	self->actualScore = 0;
	self->currentMenu = 0;
	
	Renderer2dCreate(&self->r2d, self->window);
	
}



void interface2dObjectAdd(Interface2d* self)
{
	self->windowHeigt = WindowGetHeight(self->window);
	self->windowWidth = WindowGetWidth(self->window);

	Object2dDataCreate(&self->mainMenu, make_vec2((self->windowWidth)/2, (self->windowHeigt)/2), make_vec2(self->windowWidth, self->windowHeigt));

	
	/*

	Object2dDataCreate(&self->buttonHistory, make_vec2(500, 100), make_vec2(300, 75));
	Object2dDataCreate(&self->buttonClear, make_vec2(300, 50), make_vec2(175, 50));

	Object2dDataCreate(&self->buttonBack, make_vec2(100, 50), make_vec2(175, 50));
	*/
	
}



void interface2dFrame(Interface2d* self)
{
	/*
	if (WindowGetKey(self->window, GLFW_KEY_ESCAPE) && self->currentMenu != 1)
	{
		self->currentMenu = 1;
	}
	*/

	switch (self->currentMenu)
	{
		case MAIN_MENU:
			interface2dMainMenu(self);
			break;

		case HISTORIC_MENU:
			interface2dHistoricMenu(self);
			break;

		case GAME_MENU:
			interface2dGameMenu(self);
			break;

		case PAUSE_MENU:
			interface2dPauseMenu(self);
			break;

		default:
			break;
	}
}




void interface2dMainMenu(Interface2d* self)
{
	Object2dData button;

	Renderer2dUpdate(&self->r2d);

	Renderer2dColor(&self->r2d, make_vec4(.5f, .5f, .4f, 1), &self->mainMenu);


	Renderer2dText(&self->r2d, "Best Score :", 300, 200, self->windowHeigt * 0.07);
	Renderer2dText(&self->r2d, "Average Score :", 600, 200, self->windowHeigt * 0.07);

	
	Object2dDataCreate(&button, make_vec2((self->windowWidth) / 2, (self->windowHeigt) / 2), make_vec2((self->windowWidth) * 0.35, (self->windowHeigt) * 0.15));
	if (Renderer2dColorButton(&self->r2d, "Commencer", ButtonColor(make_vec4f(1)), &button))
	{
		printf("***Game Start***");
		self->currentMenu = 2;
	}

	if (Renderer2dColorButton(&self->r2d, "Historique", ButtonColor(make_vec4f(1)), &button))
	{
		printf("***Hystorique***");
		self->currentMenu = 1;
	}

	Object2dDataCreate(&button, make_vec2((self->windowWidth) * 0.90, 50), make_vec2(175, 50));
	if (Renderer2dColorButton(&self->r2d, "Quitter", ButtonColor(make_vec4f(1)), &button))
	{
		printf("***Game Close***");
		glfwSetWindowShouldClose(self->window->ptr, GLFW_TRUE);
	}
}




void interface2dHistoricMenu(Interface2d* self)
{
	Renderer2dUpdate(&self->r2d);

	Renderer2dColor(&self->r2d, make_vec4(.5f, .5f, .5f, .8f), &self->mainMenu);

	/*
	if (Renderer2dColorButton(&self->r2d, "Retour", ButtonColor(make_vec4f(1)), &self->buttonBack))
	{
		printf("***Retour***");
		self->currentMenu = 0;
	}

	if (Renderer2dColorButton(&self->r2d, "Clear", ButtonColor(make_vec4f(1)), &self->buttonClear))
	{
		printf("***Clear***");
		Rendere2dClearHistory(self);
	}
	*/
}




void interface2dGameMenu(Interface2d* self)
{
	self->actualScore += 1;

	sprintf_s(self->buffer, 200, "Actual Score : %d", self->actualScore);
	Renderer2dText(&self->r2d, self->buffer, 10, 10, 50);
}

void interface2dPauseMenu(Interface2d* self)
{

	Renderer2dUpdate(&self->r2d);

	Renderer2dColor(&self->r2d, make_vec4(.5f, .5f, .4f, 1), &self->mainMenu);

	/*
	if (Renderer2dColorButton(&self->r2d, "Continu", ButtonColor(make_vec4f(1)), &self->buttonStart))
	{
		printf("***Game Start***");
		self->currentMenu = 2;
	}

	if (Renderer2dColorButton(&self->r2d, "Main Menu", ButtonColor(make_vec4f(1)), &self->buttonHistory))
	{
		printf("***HMain Menu***");
		self->currentMenu = 0;
	}
	*/


}



void interface2dMenuOn(Interface2d* self)
{
	
}



void Rendere2dClearHistory(Interface2d* self)
{
}
