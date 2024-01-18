#include "Game.h"

#define ORBIT_LEFT		-1
#define ORBIT_CENTER	 0
#define ORBIT_RIGHT		 1

#define EARTH_SPEED				.03f
#define EARTH_SIZE				100.f
#define SHIP_ORBIT_DISTANCE		(EARTH_SIZE + 15.f)
#define CAM_RELATIVE_HEIGHT		15.f
#define ORBITS_OFFSET			10.f

int KeyIsUp(Window* window, int *buffer, int GLFW_KEY) {
	int ret = (*buffer && !WindowGetKey(window, GLFW_KEY));
	*buffer = WindowGetKey(window, GLFW_KEY);
	return ret;
}

int KeyIsDown(Window* window, int* buffer, int GLFW_KEY) {
	int ret = (!*buffer && WindowGetKey(window, GLFW_KEY));
	*buffer = WindowGetKey(window, GLFW_KEY);
	return ret;
}

void trajectoryFunction(double speed, double t, vec4* v) {
	*v = (vec4){
		cos(t * speed) * SHIP_ORBIT_DISTANCE,
		v->y,
		sin(t * speed) * SHIP_ORBIT_DISTANCE,
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
	self->shipSpeed = 0;
	self->shipOrbit = ORBIT_CENTER;

	Object3dCreate(&self->earth, &meshes->earth, make_vec4f(0.f), make_vec4f(100.f), make_vec4(0, 1, 0, 1), 0);
	Object3dHitboxCreate(&self->ship, &meshes->ship, make_vec4f(0.f), make_vec4f(2.f), make_vec4(0, 1, 0, 1), 0);
	Object3dHitboxCreate(&self->satellite, &meshes->satellite, make_vec4(0, 0, 10, 0), make_vec4f(.4f), make_vec4(0.2, .1, 0.8, 1), 0);

	Hitbox shipHitbox;
	HitboxCreate(&shipHitbox, 3, make_vec3(0, 5, 0, 0));
	Hitbox satHitbox;
	HitboxCreate(&satHitbox, 2, make_vec3f(0));

	Object3dHitboxAddHitbox(&self->ship, &shipHitbox);
	Object3dHitboxAddHitbox(&self->satellite, &satHitbox);
	
	self->virtualCamPos = Vec4Add(self->ship.obj.position, make_vec4(0, 0, 0, 1));
	self->cameraPtr->zFar = 1000.f;
	self->cameraPtr->zNear = 4.f;
}

void GameFrame(Game* self)
{
	//check that the game is running
	if (!self->isStarted) return;
	
	//get time
	static double t = 0;
	if (self->isStarted) t = glfwGetTime();
	self->score = (unsigned)((t - self->startTime) * 100.); //score is time in cs
	
	//earth
	self->earth.rotationAngle += self->windowPtr->deltaTime * EARTH_SPEED;

	//camera
	const vec4 upVector = normalize(self->ship.obj.position); //! earth is at zero zero zero
	const vec4 camToShipVector = Vec4Sub(self->ship.obj.position, self->virtualCamPos);

	self->cameraPtr->upVector = upVector;
	
	self->cameraPtr->look = Vec4Add(Vec4Multiplyf(self->cameraPtr->position, -1), Vec4Multiplyf(camToShipVector, 30));
	self->virtualCamPos = Vec4Add(self->virtualCamPos, Vec4Multiplyf(camToShipVector, 20 * self->windowPtr->deltaTime * self->shipSpeed));
	self->cameraPtr->position = Vec4Add(Vec4Add(self->virtualCamPos, Vec4Multiplyf(upVector, CAM_RELATIVE_HEIGHT)), Vec4Multiplyf(normalize(camToShipVector), -2.f));
	self->virtualCamPos.y = self->ship.obj.position.y;

	//hitboxes
	Object3dHitboxUpdate(&self->ship);
	Object3dHitboxUpdate(&self->satellite);

	//ship
	trajectoryFunction(self->shipSpeed, t, &self->ship.obj.position);
	self->ship.obj.rotationAxis = upVector;
	self->ship.obj.rotationAngle = PI * -.5f;


	static int lb = 0, rb = 0;
	if (KeyIsDown(self->windowPtr, &lb, GLFW_KEY_A) && self->shipOrbit != ORBIT_LEFT) {
		self->shipOrbit--;
	}
	if (KeyIsDown(self->windowPtr, &rb, GLFW_KEY_D) && self->shipOrbit != ORBIT_RIGHT) {
		self->shipOrbit++;
	}
	if (WindowGetKey(self->windowPtr, GLFW_KEY_SPACE)) {
		self->shipSpeed += .00004;
	}

	if (Object3dHitboxCollide(&self->ship, &self->satellite))
		self->isStarted = 0;

	//ship orbit movement
	const float destPos = self->shipOrbit * ORBITS_OFFSET;
	self->ship.obj.position.y += (destPos - self->ship.obj.position.y) * .025f;

	//satellites

	trajectoryFunction(.5, -t, &self->satellite.obj.position);
	self->satellite.obj.rotationAngle += self->windowPtr->deltaTime * 4;

}

void GameStart(Game* self) 
{
	self->isStarted = 1;
	self->startTime = glfwGetTime();
	self->score = 0;
	self->shipSpeed = .1;

	RendererAdd(self->rendererPtr, &self->earth);
	RendererAdd(self->rendererPtr, &self->ship.obj);
	RendererAdd(self->rendererPtr, &self->satellite.obj);

	RendererSetAmbientLight(self->rendererPtr, 0.2f);
	RendererSetSunDirection(self->rendererPtr, make_vec3(1, 0, 0));
}

void GameStop(Game* self)
{
	self->isStarted = 0;

	RendererRemove(self->rendererPtr, &self->earth);
	RendererRemove(self->rendererPtr, &self->ship.obj);
	RendererRemove(self->rendererPtr, &self->satellite.obj);
}