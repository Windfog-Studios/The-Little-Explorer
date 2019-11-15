#ifndef _j1ENTITY_H
#define _j1ENTITY_H
#include "j1EntityManager.h"
#include "j1Collision.h"

class j1Entity : public j1EntityManager
{
public:
	j1Entity();
	virtual ~j1Entity();

	virtual bool Awake() {};
	virtual bool Start() {};
	virtual bool PreUpdate() {};
	virtual bool Update(float dt) {};
	virtual bool PostUpdate() {};
	virtual bool CleanUp() {};
	virtual void OnCollision(Collider* c1, Collider* c2) {};

public:
	fPoint position;
};

#endif // !_j1ENTITY_H

