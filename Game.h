#pragma once
#include <Renderer.h>
#include "MeshesLoader.h"

typedef struct Game
{
	Renderer* rendererPtr;
	Window* windowPtr;
	Camera* cameraPtr;
	MeshesLoader* meshesPtr;

	int isStarted;
	double startTime;
	unsigned score;
	Object3d earth;
	Object3d ship;
}Game;

void GameCreate(Game* self, Renderer* renderer, MeshesLoader* meshes);
void GameFrame(Game* self);
void GameStart(Game* self);
void GameStop(Game* self);