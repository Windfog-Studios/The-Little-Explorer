#include "j1Collectible.h"
#include "j1Render.h"

j1Collectible::j1Collectible() : j1Entity(EntityType::COLLECTIBLE) {
	
	name.create("collectible");
	type = EntityType::COLLECTIBLE;
}

j1Collectible::~j1Collectible(){}

bool j1Collectible::Awake(pugi::xml_node& config) {
	bool ret = true;

	return true;
}

bool j1Collectible::PostUpdate() {
	bool ret = true;
	//ret = App->render->Blit(texture, position.x, position.y);
	return ret;
}