#include "j1WalkingEnemy.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1EntityManager.h"
#include "j1Collision.h"

j1WalkingEnemy::j1WalkingEnemy() :j1Entity(EntityType::WALKING_ENEMY) {
	name.create("WalkingEnemy");
	texture = App->tex->Load("sprites/characters/Enemies/knight_spritesheet.png");
	current_animation = &idle;
	idle.PushBack({ 16,34,27,30 });
	collider = App->collision->AddCollider({ 16,34,27,30 },COLLIDER_ENEMY,(j1Module*)App->entities);
	raycast = App->collision->AddCollider({ 16,34,20,5 }, COLLIDER_ENEMY, (j1Module*)App->entities);
	lastPosition = position;
}

j1WalkingEnemy::~j1WalkingEnemy() {
	App->tex->UnLoad(texture);
	texture = nullptr;
	collider->to_delete = true;
	collider = nullptr;
	raycast->to_delete = true;
	raycast = nullptr;
}

bool j1WalkingEnemy::Update(float dt) {
	bool ret = true;
	lastPosition = position;
	current_speed.x = -50.0f;

	collider->SetPos(position.x, position.y);
	raycast->SetPos(collider->rect.x + collider->rect.w * 0.5f - raycast->rect.w * 0.5f, position.y + current_animation->GetCurrentFrame().h);

	position.x += current_speed.x * dt;

	return ret;
}

bool j1WalkingEnemy::PostUpdate() {
	bool ret = true;
	App->render->Blit(texture, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	LOG("Walking Enemy Blit");
	return ret;
}

void j1WalkingEnemy::OnCollision(Collider* c1, Collider* c2) {
	switch (c2->type)
	{
	case COLLIDER_WALL:
		position = lastPosition;
		break;
	default:
		break;
	}
}