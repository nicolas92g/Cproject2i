#include "Game.h"
#include <time.h>

#define ORBIT_LEFT		-1
#define ORBIT_CENTER	 0
#define ORBIT_RIGHT		 1

#define EARTH_SPEED				.03f
#define EARTH_SIZE				100.f
#define SHIP_ORBIT_DISTANCE		(EARTH_SIZE + 15.f)
#define CAM_RELATIVE_HEIGHT		15.f
#define ORBITS_OFFSET			10.f

#define GAME_SAVE_FORMAT	"%u;%lf"

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

unsigned hash(unsigned x) {
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = (x >> 16) ^ x;
	return x;
}

int getOrbit(char hexabit, unsigned difficulty) {
	return hexabit <= (3 * difficulty) ? hexabit / difficulty : 0;
}

void trajectoryFunction(double speed, double t, vec4* v) {
	*v = (vec4){
		cos(t * speed) * SHIP_ORBIT_DISTANCE,
		v->y,
		sin(t * speed) * SHIP_ORBIT_DISTANCE,
		1
	};
}

void reallocSatellites(Game* self, size_t size) {

	if (size <= self->satellitesReserve) return;

	Satellite* newAlloc = malloc(size * sizeof(Satellite));
	if (!newAlloc) {
		printf("Failed to allocate memory for satellites\n");
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0; i < self->satellitesCount; i++)
	{
		newAlloc[i] = self->satellites[i];
	}

	if (self->satellites) free(self->satellites);

	self->satellites = newAlloc;
	self->satellitesReserve = size;
}

void removeFirstSatellite(Game* self) {
	//check that there is a satellite to remove
	if (!self->satellitesCount) return;

	//remove
	RendererRemove(self->rendererPtr, &self->satellites[0].pObj->obj);
	free(self->satellites[0].pObj);

	self->satellitesCount--;
	for (size_t i = 0; i < self->satellitesCount; i++)
	{
		self->satellites[i] = self->satellites[i + 1];
	}
}

void appendSatellite(Game* self, const Satellite* newSat) {
	//check memory allocation
	if (self->satellitesCount == self->satellitesReserve) {
		reallocSatellites(self, self->satellitesReserve + 100);
	}

	//add sat
	self->satellites[self->satellitesCount] = *newSat;
	RendererAdd(self->rendererPtr, &self->satellites[self->satellitesCount].pObj->obj);
	self->satellitesCount++;
}

void GameCreate(Game* self, Renderer* renderer, MeshesLoader* meshes)
{
	self->rendererPtr = renderer;
	self->meshesPtr = meshes;
	self->windowPtr = renderer->windowPtr;
	self->cameraPtr = renderer->cameraPtr;
	self->isStarted = 0;
	self->satellites = NULL;
	self->cameraPtr->zFar = 1000.f;
	self->cameraPtr->zNear = 4.f;

	Object3dCreate(&self->earth, &meshes->earth, make_vec4f(0.f), make_vec4f(100.f), make_vec4(0, 1, 0, 1), 0);
	Object3dHitboxCreate(&self->ship, &meshes->ship, make_vec4f(0.f), make_vec4f(2.f), make_vec4(0, 1, 0, 1), 0);

	Hitbox shipHitbox;
	HitboxCreate(&shipHitbox, 2, make_vec3f(0));
	Object3dHitboxAddHitbox(&self->ship, &shipHitbox);

	RendererSetAmbientLight(self->rendererPtr, 0.2f);
	RendererSetSunDirection(self->rendererPtr, make_vec3(1, 0, 0));
}

void GameFrame(Game* self)
{
	//check that the game is running
	if (!self->isStarted) return;
	
	//get time
	if (self->isStarted) self->t += self->windowPtr->deltaTime;
	self->score = (unsigned)(self->t * 100.); //score is time in cs
	
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

	//ship
	self->shipSpeed = .1 + self->t / 500.0;
	Object3dHitboxUpdate(&self->ship);
	trajectoryFunction(self->shipSpeed, self->t, &self->ship.obj.position);
	self->ship.obj.rotationAxis = upVector;
	self->ship.obj.rotationAngle = PI * -.5f;


	static int lb = 0, rb = 0;
	if (KeyIsDown(self->windowPtr, &lb, GLFW_KEY_A) && self->shipOrbit != ORBIT_LEFT) {
		self->shipOrbit--;
	}
	if (KeyIsDown(self->windowPtr, &rb, GLFW_KEY_D) && self->shipOrbit != ORBIT_RIGHT) {
		self->shipOrbit++;
	}

	//collision
	for (size_t i = 0; i < self->satellitesCount; i++)
	{
		Object3dHitboxUpdate(self->satellites[i].pObj);

		if (Object3dHitboxCollide(&self->ship, self->satellites[i].pObj)) {
			self->isStarted = 0;
		}
			
	}

	//ship orbit movement
	const float destPos = self->shipOrbit * ORBITS_OFFSET;
	self->ship.obj.position.y += (destPos - self->ship.obj.position.y) * .025f;

	//satellites
	for (size_t i = 0; i < self->satellitesCount; i++)
	{
		//trajectory
		const double l = self->satellites[i].launchTime;
		trajectoryFunction(self->shipSpeed, (l + 15) - (self->t - l) * 5, &self->satellites[i].pObj->obj.position);
		
		//rotation
		self->satellites[i].pObj->obj.rotationAngle += self->windowPtr->deltaTime * 4;
	}


	char buf[100];
	sprintf_s(buf, 100, "score = %lf\n", self->t);// self->score);

	//remove satellites
	if (self->satellitesCount > 0 && self->satellites[0].launchTime + 3 < self->t) {
		removeFirstSatellite(self);
	}
	

	//pseudo random add sats
	static unsigned currentSecond = 0;
	if (currentSecond != (unsigned)self->t && self->satellitesCount < 100 && self->t > 3 + self->importedTime) {

		unsigned digest = hash((unsigned)self->t + self->seed);

		char bits[8];

		bits[0] = (digest & 0xf);
		bits[1] = (digest & 0xf0) >> 4;
		bits[2] = (digest & 0xf00) >> 8;
		bits[3] = (digest & 0xf000) >> 12;
		bits[4] = (digest & 0xf0000) >> 16;
		bits[5] = (digest & 0xf00000) >> 20;
		bits[6] = (digest & 0xf000000) >> 24;
		bits[7] = (digest & 0xf0000000) >> 28;

		
		int orbits[3] = {0};
		for (size_t i = 0; i < 4; i++)
		{
			const auto o = getOrbit(bits[i], min(self->difficulty, 5));
			// add a sat if there is no satellite on the c orbit and if there is less than 2 sats
			if (o && !orbits[o - 1]) {

				Satellite sat;
				sat.launchTime = self->t + bits[i + 4] / 20.;
				sat.pObj = malloc(sizeof(Object3dHitbox));

				//mesh selection (satellite or asteroid ?)
				Mesh* mesh;
				vec4 scale;
				if (bits[i + 4] + bits[i + 3] > 15) {
					mesh = &self->meshesPtr->satellite;
					scale = make_vec4f(.5f);
				}
				else {
					mesh = &self->meshesPtr->asteroid;
					scale = make_vec4f(2.f);
				}

				Object3dHitboxCreate(sat.pObj, 
					mesh, 
					make_vec4(100, (o - 2) * ORBITS_OFFSET, 0, 0), 
					scale, 
					make_vec4(rand() % 100 / 100., rand() % 100 / 100., rand() % 100 / 100., 0), 
					90
				);

				Hitbox satHitbox;
				HitboxCreate(&satHitbox, 2, make_vec3f(0));
				Object3dHitboxAddHitbox(sat.pObj, &satHitbox);

				appendSatellite(self, &sat);
				orbits[o - 1] = 1;
			}
		}
	}
	currentSecond = (unsigned)self->t;


	//TO REMOVE !!!!!
	WindowSetTitle(self->windowPtr, buf);
}

void GameStart(Game* self)
{
	self->satellites = NULL;
	self->isStarted = 1;
	self->score = 0;
	self->shipSpeed = .1;
	self->difficulty = 2;
	self->shipOrbit = ORBIT_CENTER;
	self->importedTime = 0;

	self->ship.obj.position = make_vec4f(0.f);
	self->virtualCamPos = make_vec4f(0.f);

	srand(time(NULL) + (unsigned)(glfwGetTime() * 1000.));

	FILE* save = fopen("GameSave.txt", "rb");
	if (save) {
		if (fscanf_s(save, GAME_SAVE_FORMAT, &self->seed, &self->importedTime)){
			self->t = max(0, self->importedTime - 3); // remove 3 seconds
		}
		else {
			self->t = 0;
			self->seed = (unsigned)abs(rand());
		}
		fclose(save);

		FILE* remove = fopen("GameSave.txt", "wb");
		if (remove) fclose(remove);
	}
	else {
		self->seed = (unsigned)abs(rand());
		self->t = 0;
	}

	printf("seed = %u", self->seed);
	

	self->satellitesReserve = 0;
	self->satellitesCount = 0;
	reallocSatellites(self, 100);

	RendererAdd(self->rendererPtr, &self->earth);
	RendererAdd(self->rendererPtr, &self->ship.obj);
}

void GameStop(Game* self)
{
	self->isStarted = 0;

	RendererRemove(self->rendererPtr, &self->earth);
	RendererRemove(self->rendererPtr, &self->ship.obj);

	while (self->satellitesCount)
	{
		removeFirstSatellite(self);
	}

	free(self->satellites);
	self->satellitesReserve = 0;
}

void GameSave(Game* self)
{
	FILE* f = fopen("GameSave.txt", "wb");
	if (!f) return;

	fprintf(f, GAME_SAVE_FORMAT, self->seed, self->t);

	fclose(f);
}

int GameIsSavedGameExists(){
	FILE* f = fopen("GameSave.txt", "rb");
	if (!f) return 0;
	double lf;
	unsigned u;
	const int ret = fscanf_s(f, GAME_SAVE_FORMAT, &u, &lf);
	fclose(f);
	return ret;
}
