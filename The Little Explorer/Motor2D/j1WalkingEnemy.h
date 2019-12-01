#ifndef _j1WALKING_ENEMY_H
#define _j1WALKING_ENEMY_H
#include "j1Entity.h"
#include "j1Audio.h"

class j1WalkingEnemy : public j1Entity
{
public:
	j1WalkingEnemy();
	virtual ~j1WalkingEnemy();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();

	void OnCollision(Collider* c1, Collider* c2);

	void MovementControl(float dt) {}

	bool Save(pugi::xml_node& data) const { return true; }
	bool Load(pugi::xml_node& data) { return true; }

private:
	float attacking_range = 1;

public:
	uint die2_fx;           
	p2SString die2_fx_path;

	uint Enemy_attack_fx;
	p2SString Enemy_attack_fx_path;

};

#endif // !_j1WALKING_ENEMY_H

