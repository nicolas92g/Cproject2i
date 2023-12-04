#include "Game.h"

#define EARTH_SPEED				.03f
#define EARTH_SIZE				100.f
#define SHIP_ORBIT_DISTANCE		(EARTH_SIZE + 20.f)

vec4 shipTrajectoryFunction(double t) {
	const float shipSpeed = .1f;
	return (vec4) { 
		cos(t * shipSpeed)* SHIP_ORBIT_DISTANCE, 
		0, 
		sin(t * shipSpeed)* SHIP_ORBIT_DISTANCE, 
		1 
	};
}

void GameCreate(Game* self, Renderer* renderer, MeshesLoader* meshes)
{
	self->rendererPtr = renderer;
	self->meshesPtr = meshes;
	self->windowPtr = renderer->windowPtr;
	self->cameraPtr = renderer->cameraPtr;
	self->isStarted = 0;

	Object3dCreate(&self->earth, &meshes->earth, make_vec4f(0.f), make_vec4f(100.f), make_vec4(0, 1, 0, 1), 0);
	Object3dCreate(&self->ship, &meshes->ship, make_vec4f(0.f), make_vec4f(2.f), make_vec4(0, 1, 0, 1), 0);
	self->cameraPtr->zFar = 1000.f;
	self->cameraPtr->zNear = 1.f;
}

void GameFrame(Game* self)
{
	//check that the game is running
	if (!self->isStarted) return;


	self->score = (unsigned)(glfwGetTime() - self->startTime);
	
	//get time
	const double t = glfwGetTime();
	
	//earth
	self->earth.rotationAngle += self->windowPtr->deltaTime * EARTH_SPEED;

	//camera and ship
	self->ship.position = shipTrajectoryFunction(t);

	const vec4 tangentVector = Vec4Sub(shipTrajectoryFunction(t + 10), self->ship.position);
	const vec4 upVector = normalize(self->ship.position); //! earth is at zero zero zero
	const vec4 rightVector = crossProduct(tangentVector, upVector);

	self->ship.rotationAxis = upVector;
	self->ship.rotationAngle = PI * -.5f;

	self->cameraPtr->upVector = upVector;
	self->cameraPtr->look = tangentVector;
	self->cameraPtr->position = Vec4Add(Vec4Add(self->ship.position, Vec4Multiplyf(upVector, 2)), Vec4Multiplyf(normalize(tangentVector), -10.f));
	

	//CameraBasicControls(self->cameraPtr, .005f, 10, 10)
}

void GameStart(Game* self) 
{
	self->isStarted = 1;
	self->startTime = glfwGetTime();
	self->score = 0;

	RendererAdd(self->rendererPtr, &self->earth);
	RendererAdd(self->rendererPtr, &self->ship);

	RendererSetAmbientLight(self->rendererPtr, 0.1);
	RendererSetSunDirection(self->rendererPtr, make_vec3(1, 0, 0));
}

void GameStop(Game* self)
{
	self->isStarted = 0;

	RendererRemove(self->rendererPtr, &self->earth);
	RendererRemove(self->rendererPtr, &self->ship);
}
