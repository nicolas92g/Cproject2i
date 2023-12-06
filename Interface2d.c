#include "Interface2d.h"


void interface2dCreate(Interface2d* self, Window* window, Camera* camera)
{
	self->camera = camera;
	self->window = window;
	Renderer2dCreate(&self->r2d, self->window);
	self->isMenuOn = 1;
	
}

void interface2dObjectAdd(Interface2d* self)
{
	Object2dDataCreate(&self->mainMenu, make_vec2(500, 300), make_vec2(1000, 600));

	Object2dDataCreate(&self->buttonStart, make_vec2(500, 400), make_vec2(375, 100));
	Object2dDataCreate(&self->buttonClose, make_vec2(800, 50), make_vec2(175, 50));

	Object2dDataCreate(&self->buttonHistory, make_vec2(500, 100), make_vec2(300, 75));
	Object2dDataCreate(&self->buttonClear, make_vec2(300, 50), make_vec2(175, 50));

	Object2dDataCreate(&self->buttonBack, make_vec2(100, 50), make_vec2(175, 50));

	

	
}

void interface2dFrame(Interface2d* self)
{

	if (WindowGetKey(self->window, GLFW_KEY_ESCAPE) && self->isMenuOn != 1)
	{
		self->isMenuOn = 1;
	}


	if (self->isMenuOn == 1)
	{
		interface2dMenuOn(self);
	}
	else if(self->isMenuOn == 2)
	{
		Renderer2dHistory(self);

	}
	else {
		
		interface2dMenuOFF(self);
	}
}

void interface2dMenuOn(Interface2d* self)
{
	Renderer2dUpdate(&self->r2d);

	Renderer2dColor(&self->r2d, make_vec4(.5f, .5f, .5f, .8f), &self->mainMenu);

	Renderer2dText(&self->r2d, "Best Score :" , 300, 200, 40);
	Renderer2dText(&self->r2d, "Average Score :", 600, 200, 40);

	if (Renderer2dColorButton(&self->r2d, "Commencer", ButtonColor(make_vec4f(1)), &self->buttonStart))
	{
		printf("***Game Start***");
		self->isMenuOn = 0;
	}

	if (Renderer2dColorButton(&self->r2d, "Historique", ButtonColor(make_vec4f(1)), &self->buttonHistory))
	{
		printf("***Hystorique***");
		self->isMenuOn = 2;
	}

	if (Renderer2dColorButton(&self->r2d, "Quitter", ButtonColor(make_vec4f(1)), &self->buttonClose))
	{
		printf("***Game Close***");
		glfwSetWindowShouldClose(self->window->ptr, GLFW_TRUE);
	}
}

void interface2dMenuOFF(Interface2d* self)
{
	CameraBasicControls(self->camera, .005f, 2, 10);

	Renderer2dText(&self->r2d, "Actual Score", 10, 10, 50);
}

void Renderer2dHistory(Interface2d* self)
{
	Renderer2dUpdate(&self->r2d);

	Renderer2dColor(&self->r2d, make_vec4(.5f, .5f, .5f, .8f), &self->mainMenu);

	Renderer2dText(&self->r2d, "Hystorique", 500, 400, 40);

	if (Renderer2dColorButton(&self->r2d, "Retour", ButtonColor(make_vec4f(1)), &self->buttonBack))
	{
		printf("***Retour***");
		self->isMenuOn = 1;
	}

	if (Renderer2dColorButton(&self->r2d, "Clear", ButtonColor(make_vec4f(1)), &self->buttonClear))
	{
		printf("***Clear***");
		Rendere2dClearHistory(self);
	}
}

void Rendere2dClearHistory(Interface2d* self)
{
}
