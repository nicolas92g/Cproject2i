#pragma once
#include <Renderer.h>
#include "MeshesLoader.h"
#include "Object3dHitbox.h"

typedef struct Satellite {
	Object3dHitbox* pObj;
	double launchTime;
}Satellite;

typedef struct Game
{
	Renderer* rendererPtr;
	Window* windowPtr;
	Camera* cameraPtr;
	MeshesLoader* meshesPtr;

	int isStarted;
	unsigned score;
	vec4 virtualCamPos;
	double shipSpeed;
	char shipOrbit; // -1, 0, or 1

	double t;
	double importedTime;
	unsigned seed;
	unsigned difficulty;

	Object3d earth;
	Object3dHitbox ship;

	Satellite* satellites;
	size_t satellitesCount;
	size_t satellitesReserve;

}Game;

void GameCreate(Game* self, Renderer* renderer, MeshesLoader* meshes);
void GameFrame(Game* self);
void GameStart(Game* self);
void GameStop(Game* self);

void GameSave(Game* self);
int GameIsSavedGameExists();
