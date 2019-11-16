#ifndef _j1ENEMY_H
#define _j1ENEMY_H
#include "j1Entity.h"

class j1Enemy : public j1Entity
{
public:
	j1Enemy();
	virtual ~j1Enemy();

	virtual bool Awake(pugi::xml_node&) { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };
	virtual void OnCollision(Collider* c1, Collider* c2) {};

public:
	
};

#endif // !_j1ENEMY_H


