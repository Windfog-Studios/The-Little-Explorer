#include "j1WalkingEnemy.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1EntityManager.h"

j1WalkingEnemy::j1WalkingEnemy() :j1Entity(EntityType::WALKING_ENEMY) {
	name.create("WalkingEnemy");
	animations.add(&idle);
	texture = App->tex->Load("sprites/characters/Enemies/knight_spritesheet.png");
	current_animation = &idle;
	idle.PushBack({ 16,34,27,30 });
}

j1WalkingEnemy::~j1WalkingEnemy() {

}

bool  j1WalkingEnemy::PostUpdate() {
	bool ret = true;
	App->render->Blit(texture, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	LOG("Walking Enemy Blit");
	return ret;
}