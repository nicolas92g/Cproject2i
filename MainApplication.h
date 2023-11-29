#pragma once
#include <Renderer.h>

typedef struct MainApplication
{
	Window window;
	Camera camera;
	Renderer renderer;

	Renderer2d r2d;
	Mesh mesh;
	Object3d obj;


}MainApplication;

void MainApplicationCreate(MainApplication* app);
void MainApplicationRun(MainApplication* app);

