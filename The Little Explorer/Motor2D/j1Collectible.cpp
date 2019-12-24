#include "j1Collectible.h"
#include "j1Render.h"
#include "j1Collision.h"

j1Collectible::j1Collectible() : j1Entity(EntityType::COLLECTIBLE) {
	
	name.create("collectible");
	type = EntityType::COLLECTIBLE;
	LoadAnimations("coin_animation.tmx");

	if (App->entities->reference_collectible != nullptr)
	{
		texture = App->entities->reference_collectible->texture;
		collider = App->collision->AddCollider({ position.x, position.y, 32,32 }, COLLIDER_COLLECTIBLE, this);
	}
	current_animation = &idle;
}

j1Collectible::~j1Collectible(){}

bool j1Collectible::Awake(pugi::xml_node& config) {
	bool ret = true;
	return true;
}

bool j1Collectible::Update(float dt) {
	bool ret = true;
	collider->SetPos(position.x, position.y);
	return ret;
}

bool j1Collectible::PostUpdate() {
	bool ret = true;
	ret = App->render->Blit(texture, position.x, position.y,&current_animation->GetCurrentFrame());
	return ret;
}