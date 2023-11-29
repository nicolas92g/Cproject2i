#include "MainApplication.h"

void frame(void* appVoidptr)
  {
	MainApplication* app = (MainApplication*)appVoidptr;

	Object2dData button;
	Object2dDataCreate(&button, make_vec2(200, 100), make_vec2(200, 40));
	
	Renderer2dUpdate(&app->r2d);
	if (Renderer2dColorButton(&app->r2d, "test button", ButtonColor(BUTTON_COLOR), &button)) printf("coucou\n\n");
	//Renderer2dColor(&app->r2d, make_vec4(1, 0, 0, 1), &button);

	CameraBasicControls(&app->camera, .005f, 2, 10);

	if (WindowGetKey(&app->window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(app->window.ptr, GLFW_TRUE);
	}
}

void MainApplicationCreate(MainApplication* app)
{
	//create context of the app
	WindowCreate(&app->window, 1000, 600, "Cproject2i");
	CameraCreate(&app->camera, &app->window, make_vec4f(0), PI * .5f);
	RendererCreate(&app->renderer, &app->window, &app->camera);

	Renderer2dCreate(&app->r2d, &app->window);
	

	WindowSetBackgroundColor(.4, .7, .8, 1);
	//glfwMaximizeWindow(app->window.ptr);

	//
	//MeshCreateFromObj(&app->mesh, "./assets/starship/startshipLowPoly.obj");
	MeshCreateFromObj(&app->mesh, "./assets/earth/earth.obj");
	Object3dCreate(&app->obj, &app->mesh, make_vec4(0, 0, 0, 1), make_vec4f(1), make_vec4(0, 1, 0, 1), 0);
	RendererAdd(&app->renderer, &app->obj);
}

void MainApplicationRun(MainApplication* app)
{
	RendererStartMainLoop(&app->renderer, frame, app);
}
