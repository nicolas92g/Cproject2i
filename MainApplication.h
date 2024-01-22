#pragma once
#include <Renderer.h>
#include "MeshesLoader.h"
#include "Game.h"
#include "Interface2d.h"

typedef struct MainApplication
{
	Window window;
	Camera camera;
	Renderer renderer;

	MeshesLoader meshes;
	Game game;

	Renderer2d r2d;
	Texture texture;

	Interface2d interface2d;

}MainApplication;

void MainApplicationCreate(MainApplication* app);
void MainApplicationRun(MainApplication* app);

