#include "MainApplication.h"
#include "NasaApiReader.h"
#include <stb_image.h>

void frame(void* appVoidptr)
  {
	static unsigned long long f = 0;
	glDisable(GL_CULL_FACE);

	MainApplication* app = (MainApplication*)appVoidptr;

	GameFrame(&app->game);

	


	//close window with escape
	if (WindowGetKey(&app->window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(app->window.ptr, GLFW_TRUE);
	}

	//print fps
	const unsigned fps = 1.f / (app->window.deltaTime);
	//if  (!isinf(fps) && !(f % (fps + 1))) printf("%d fps\n", fps);
	f++;
}

void MainApplicationCreate(MainApplication* app)
{
	//create context of the app
	WindowCreate(&app->window, 1000, 600, "Cproject2i");
	CameraCreate(&app->camera, &app->window, make_vec4(0, 0, 00, 1), PI * .5f);
	RendererCreate(&app->renderer, &app->window, &app->camera);
	

	WindowSetBackgroundColor(.01, .01, .01, 1);
	glfwSwapInterval(1);
	glfwMaximizeWindow(app->window.ptr);

	//

	NasaApiReader api;
	NasaApiReaderCreate(&api);
	Texture img = NasaApiReaderGetTodayImage(&api);

	

	MeshesLoaderCreate(&app->meshes);
	GameCreate(&app->game, &app->renderer, &app->meshes);
	GameStart(&app->game);
}

void MainApplicationRun(MainApplication* app)
{
	RendererStartMainLoop(&app->renderer, frame, app);
}
