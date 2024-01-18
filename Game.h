#pragma once
#include <Renderer.h>
#include "MeshesLoader.h"
#include "Object3dHitbox.h"

typedef struct Game
{
	Renderer* rendererPtr;
	Window* windowPtr;
	Camera* cameraPtr;
	MeshesLoader* meshesPtr;

	int isStarted;
	double startTime;
	unsigned score;
	vec4 virtualCamPos;
	double shipSpeed;
	char shipOrbit; // -1, 0, or 1

	Object3d earth;
	Object3dHitbox ship;

	Object3dHitbox satellite;
}Game;

void GameCreate(Game* self, Renderer* renderer, MeshesLoader* meshes);
void GameFrame(Game* self);
void GameStart(Game* self);
void GameStop(Game* self);