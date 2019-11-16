#ifndef _j1ENEMY_H
#define _j1ENEMY_H
#include "j1Entity.h"

class j1Enemy : public j1Entity
{
public:
	j1Enemy();
	virtual ~j1Enemy();

	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();
	virtual void OnCollision(Collider* c1, Collider* c2);

public:
	
};

#endif // !_j1ENEMY_H


