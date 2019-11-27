#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

j1Entity::j1Entity(EntityType type) : j1EntityManager() {
}

j1Entity::~j1Entity() {

}

bool j1Entity::Awake(pugi::xml_node& config) {
	
	//config = config.child("")

	return true;
}

bool j1Entity::Start() {
	bool ret = true;

	return ret;
}

bool j1Entity::PreUpdate() {
	bool ret = true;
	return ret;
}

bool j1Entity::Update(float dt) {
	bool ret = true;
	return ret;
}

bool j1Entity::PostUpdate() {
	bool ret = true;

	return ret;
}

void j1Entity::PathfindtoPlayer(int range, j1Entity* player) {

	//if the player is close we create a path to him
	if (abs(player->position.x - position.x) < range)
	{
		iPoint origin = App->map->WorldToMap(position.x, position.y);
		iPoint destination = App->map->WorldToMap(player->position.x, player->position.y);
		App->pathfinding->CreatePath(origin, destination);
		going_after_player = true;
	}
	else { going_after_player = false; }

	//pathfinding debug
	if (going_after_player)
	{
		int x, y;
		SDL_Rect Debug_rect = { 0,0,32,32 };

		path_to_player = App->pathfinding->GetLastPath();

		for (uint i = 0; i < path_to_player->Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(path_to_player->At(i)->x, path_to_player->At(i)->y);
			Debug_rect.x = pos.x;
			Debug_rect.y = pos.y;
			if (App->collision->debug)App->render->DrawQuad(Debug_rect, 90, 850, 230, 80);
		}
	}

	//try to reach the player
	if ((path_to_player != nullptr) && (path_to_player->Count() != 0))
	{
		int i = 0;
		iPoint current_map_position = App->map->WorldToMap(position.x, position.y);
		iPoint tile_to_go;
		tile_to_go.x = path_to_player->At(i)->x;
		tile_to_go.y = path_to_player->At(i)->y;

		if (current_map_position.x == tile_to_go.x)
		{
			i++;
			if (i > 1)
			{
				tile_to_go = App->map->WorldToMap(path_to_player->At(i)->x, path_to_player->At(i)->y);
			}
		}

		if (current_map_position.x > tile_to_go.x) {
			LOG("Going left");
			state = RUN_BACKWARD;
		}
		if (current_map_position.x < tile_to_go.x) {
			LOG("Going right");
			state = RUN_FORWARD;
		}
		if (current_map_position.y > tile_to_go.y) {
			LOG("Going up");
			//position.y -= 30;
		}
		if (current_map_position.y < tile_to_go.y) {
			LOG("Going down");
		}
	}
}
