#ifndef _j1WALKING_ENEMY_H
#define _j1WALKING_ENEMY_H
#include "j1Entity.h"
#include "p2DynArray.h"

class j1WalkingEnemy : public j1Entity
{
public:
	j1WalkingEnemy();
	virtual ~j1WalkingEnemy();

	bool Awake(pugi::xml_node&) { return true; }
	bool Update(float dt);
	bool PostUpdate();

	void OnCollision(Collider* c1, Collider* c2);

	void MovementControl(float dt) {}

	bool Save(pugi::xml_node& data) const { return true; }
	bool Load(pugi::xml_node& data) { return true; }

	bool LoadAnimations() { return true; }

	void PathfindtoPlayer(int range);

private:
	int path_minimum = 100;
	int path_maximum = 125;
	bool going_after_player = false;

	const p2DynArray<iPoint>* path_to_player;
};

#endif // !_j1WALKING_ENEMY_H

