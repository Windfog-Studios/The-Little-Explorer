#include "j1Enemy.h"

j1Enemy::j1Enemy() : j1Entity(EntityType::ENEMY){

}

j1Enemy::~j1Enemy(){}

bool j1Enemy::Update(float dt) {
	bool ret = true;
	return ret;
}

bool j1Enemy::PostUpdate() {
	bool ret = true;
	return ret;
}

bool j1Enemy::CleanUp() {
	bool ret = true;
	return ret;
}

void j1Enemy::OnCollision(Collider* c1, Collider* c2) {

}