#include "MainApplication.h"
#include "NasaApiReader.h"
#include <stb_image.h>

void loadingLoop(void* appVoidptr) {
	MainApplication* app = (MainApplication*)appVoidptr;

	static int l = 1;
	if (l) { glfwMaximizeWindow(app->window.ptr); l = 0; return; }
	
	Renderer2dUpdate(&app->r2d);
	int w = WindowGetWidth(&app->window);
	int h = WindowGetHeight(&app->window);
	Object2dData imgData;
	Object2dDataCreate(&imgData, make_vec2(w/2, h/2), make_vec2(w, h));
	Renderer2dImage(&app->r2d, &app->texture, &imgData);
	glfwSetWindowShouldClose(app->window.ptr, GLFW_TRUE);
}

void frame(void* appVoidptr)
  {

	MainApplication* app = (MainApplication*)appVoidptr;

	GameFrame(&app->game);

	//close window with escape
	if (WindowGetKey(&app->window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(app->window.ptr, GLFW_TRUE);
	}
}

void MainApplicationCreate(MainApplication* app)
{
	//create context of the app
	WindowCreate(&app->window, 1000, 600, "Cproject2i");
	CameraCreate(&app->camera, &app->window, make_vec4(0, 0, 0, 1), PI * .5f);
	RendererCreate(&app->renderer, &app->window, &app->camera);

	WindowSetBackgroundColor(.01, .01, .01, 1);
	glfwSwapInterval(1);

	//
	Renderer2dCreate(&app->r2d, &app->window);
	NasaApiReader api;
	NasaApiReaderCreate(&api);
	//app->texture = NasaApiReaderGetTodayImage(&api);

}

void MainApplicationRun(MainApplication* app)
{
	//loading screen with nasa image
	RendererStartMainLoop(&app->renderer, loadingLoop, app);

	//load meshes
	MeshesLoaderCreate(&app->meshes);
	GameCreate(&app->game, &app->renderer, &app->meshes);
	GameStart(&app->game);

	//game loop
	glfwSetWindowShouldClose(app->window.ptr, GLFW_FALSE);
	RendererStartMainLoop(&app->renderer, frame, app);
}
