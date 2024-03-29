#pragma once
#include <Mesh.h>

typedef struct MeshesLoader
{
	Mesh earth;
	Mesh satellite;
	Mesh ship;
	Mesh asteroid;

}MeshesLoader;

void MeshesLoaderCreate(MeshesLoader* self);
void MeshesLoaderDestroy(MeshesLoader* self);
