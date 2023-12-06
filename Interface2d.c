#include "Interface2d.h"


void interface2dCreate(Interface2d* self, Window* window)
{
	self->window = window;
	Renderer2dCreate(&self->r2d, self->window);
	self->isMenuOn = 1;
	
}

void interface2dObjectAdd(Interface2d* self)
{
	Object2dDataCreate(&self->mainMenu, make_vec2(500, 300), make_vec2(1000, 600));
	Object2dDataCreate(&self->buttonStart, make_vec2(500, 400), make_vec2(200, 100));
	Object2dDataCreate(&self->buttonClose, make_vec2(500, 200), make_vec2(200, 100));
}

void interface2dFrame(Interface2d* self, Camera* camera)
{
	self->camera = camera;

	if (WindowGetKey(self->window, GLFW_KEY_ESCAPE) && self->isMenuOn == 0)
	{
		self->isMenuOn = 1;
	}


	if (self->isMenuOn)
	{
		interface2dMenuOn(self);
	}
	else {
		CameraBasicControls(self->camera, .005f, 2, 10);

	}
}

void interface2dMenuOn(Interface2d* self)
{
	Renderer2dUpdate(&self->r2d);
	Renderer2dColor(&self->r2d, make_vec4(4, 7, 8, 1), &self->buttonStart);
	if (Renderer2dColorButton(&self->r2d, "Commencer", ButtonColor(BUTTON_COLOR), &self->buttonStart))
	{
		printf("***Game Start***");
		self->isMenuOn = 0;
	}

	if (Renderer2dColorButton(&self->r2d, "quitter", ButtonColor(BUTTON_COLOR), &self->buttonClose))
	{
		printf("***Game Close***\n");
		glfwSetWindowShouldClose(self->window->ptr, GLFW_TRUE);
	}
}
