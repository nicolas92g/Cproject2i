#include "Object3dHitbox.h"



void Object3dHitboxCreate(Object3dHitbox* self, Mesh* mesh, vec4 position, vec4 scale, vec4 rotationAxis, float rotationAngle)
{
	Object3dCreate(&self->obj, mesh, position, scale, rotationAxis, rotationAngle);
	self->hitboxesCount = 0;
}

void Object3dHitboxAddHitbox(Object3dHitbox* self, const Hitbox* hitbox)
{
	if (self->hitboxesCount == OBJECT3D_HITBOXES_MAX_COUNT) return;
	self->hitboxes[self->hitboxesCount] = *hitbox;
	self->hitboxesCount++;
}

int Object3dHitboxCollide(const Object3dHitbox* self, const Object3dHitbox* other)
{
	for (short i = 0; i < self->hitboxesCount; i++)
	{
		for (short j = 0; j < other->hitboxesCount; j++) 
		{
			if (HitboxCollide(&self->hitboxes[i], &other->hitboxes[j])) return 1;
		}
	}
	return 0;
}

void Object3dHitboxUpdate(Object3dHitbox* self)
{
	for (size_t i = 0; i < self->hitboxesCount; i++)
	{
		self->hitboxes[i].position = make_vec3(self->obj.position.x, self->obj.position.y, self->obj.position.z);
	}
}

