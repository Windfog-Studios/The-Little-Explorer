#ifndef _j1WALKING_ENEMY_H
#define _j1WALKING_ENEMY_H
#include "j1Entity.h"

class j1WalkingEnemy : public j1Entity
{
public:
	j1WalkingEnemy();
	virtual ~j1WalkingEnemy();

	bool Awake(pugi::xml_node&) { return true; }
	bool Update(float dt) { return true; }
	bool PostUpdate();

	void OnCollision(Collider* c1, Collider* c2) {}

	void MovementControl(float dt) {}

	bool Save(pugi::xml_node& data) const { return true; }
	bool Load(pugi::xml_node& data) { return true; }

	bool LoadAnimations() { return true; }

private:
	
};

#endif // !_j1WALKING_ENEMY_H

