#ifndef _j1WALKING_ENEMY_H
#define _j1WALKING_ENEMY_H
#include "j1Entity.h"

class j1WalkingEnemy : public j1Entity
{
public:
	j1WalkingEnemy();
	virtual ~j1WalkingEnemy();

	bool Update(float dt);
	bool PostUpdate();

	void OnCollision(Collider* c1, Collider* c2);

	void MovementControl(float dt) {}

	bool Save(pugi::xml_node& data) const { return true; }
	bool Load(pugi::xml_node& data) { return true; }

	bool LoadAnimations() { return true; }

private:
	//int path_minimum = 100;
	//int path_maximum = 125;
};

#endif // !_j1WALKING_ENEMY_H

