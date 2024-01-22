#include "MeshesLoader.h"

void MeshesLoaderCreate(MeshesLoader* self)
{
	MeshCreateFromObj(&self->earth, "./assets/earth/earth.obj");
	MeshCreateFromObj(&self->ship, "./assets/starship/startshipLowPoly.obj");
	MeshCreateFromObj(&self->satellite, "./assets/satellite/satellite.obj");
	MeshCreateFromObj(&self->asteroid, "./assets/asteroid/asteroid.obj");
}

void MeshesLoaderDestroy(MeshesLoader* self)
{
	MeshDestroy(&self->earth);
	MeshDestroy(&self->ship);
	MeshDestroy(&self->satellite);
	MeshDestroy(&self->asteroid);
}