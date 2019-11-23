#ifndef _j1FLYING_ENEMY_H
#define _j1FLYING_ENEMY_H
#include "j1Entity.h"

class j1FlyingEnemy:public j1Entity
{
public:
	j1FlyingEnemy();
	virtual ~j1FlyingEnemy();

private:
		bool Awake(pugi::xml_node&) { return true; }
		bool Update(float dt) { return true; }
		bool PostUpdate() { return true; }

		void OnCollision(Collider* c1, Collider* c2){}

		bool Save(pugi::xml_node& data) const { return true; }
		bool Load(pugi::xml_node& data) { return true; }

		bool LoadAnimations() { return true; }
};

#endif // !_j1FLYING_ENEMY_H


