#pragma once
#include <Object3d.h>
#include <Hitbox.h>

#define OBJECT3D_HITBOXES_MAX_COUNT 5

typedef struct 
{
	Object3d obj;
	Hitbox hitboxes[OBJECT3D_HITBOXES_MAX_COUNT];
	short hitboxesCount;
}Object3dHitbox;

void Object3dHitboxCreate(Object3dHitbox* self, Mesh* mesh, vec4 position, vec4 scale, vec4 rotationAxis, float rotationAngle);
void Object3dHitboxAddHitbox(Object3dHitbox* self, const Hitbox* hitbox);

int Object3dHitboxCollide(const Object3dHitbox* self, const Object3dHitbox* other);
void Object3dHitboxUpdate(Object3dHitbox* self);
