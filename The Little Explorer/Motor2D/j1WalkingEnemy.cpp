#include "j1WalkingEnemy.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1EntityManager.h"
#include "j1Collision.h"

j1WalkingEnemy::j1WalkingEnemy() :j1Entity(EntityType::WALKING_ENEMY) {
	name.create("WalkingEnemy");
	animations.add(&idle);
	texture = App->tex->Load("sprites/characters/Enemies/knight_spritesheet.png");
	current_animation = &idle;
	idle.PushBack({ 16,34,27,30 });
	collider = App->collision->AddCollider({ 16,34,27,30 },COLLIDER_ENEMY,(j1Module*)App->entities);
	raycast = App->collision->AddCollider({ 16,34,20,5 }, COLLIDER_ENEMY, (j1Module*)App->entities);
}

j1WalkingEnemy::~j1WalkingEnemy() {

}

bool j1WalkingEnemy::Update(float dt) {
	bool ret = true;
	position.x--;
	collider->SetPos(position.x, position.y);
	raycast->SetPos(collider->rect.x + collider->rect.w * 0.5f - raycast->rect.w * 0.5f, position.y + current_animation->GetCurrentFrame().h);
	return ret;
}

bool  j1WalkingEnemy::PostUpdate() {
	bool ret = true;
	App->render->Blit(texture, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	LOG("Walking Enemy Blit");
	return ret;
}