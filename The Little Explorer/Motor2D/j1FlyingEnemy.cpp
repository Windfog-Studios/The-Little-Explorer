#include "j1FlyingEnemy.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

j1FlyingEnemy::j1FlyingEnemy() :j1Entity(EntityType::FLYING_ENEMY) {
	name.create("flying_enemy");
	texture = App->tex->Load("sprites/characters/Enemies/Sprite_bat.png");
	current_animation = &idle;
	idle.PushBack({ 0,66,45,26 });
	collider = App->collision->AddCollider({ 0,66,45,26 }, COLLIDER_ENEMY, (j1Module*)this);
	raycast = App->collision->AddCollider({ 16,34,20,5 }, COLLIDER_ENEMY, (j1Module*)this);
	lastPosition = position;
	player = App->entities->player;
	speed.x = 20;
	health = 50;
	flip = SDL_FLIP_HORIZONTAL;
}

j1FlyingEnemy::~j1FlyingEnemy() {
	App->entities->DestroyEntity(this);
	App->tex->UnLoad(texture);
	texture = nullptr;
	collider->to_delete = true;
	collider = nullptr;
	raycast->to_delete = true;
	raycast = nullptr;
}

/*
bool j1FlyingEnemy::Update(float dt) {
	bool ret = true;
	lastPosition = position;
	gravity = 925;


	//what to do when getting to a gap
	if (last_collider != nullptr)
	{
		if (!raycast->CheckCollision(last_collider->rect))
		{
			grounded = false;
			//current_speed.x = -current_speed.x;
		}
	}

	//guard path
	//if ((position.x < path_minimum)||(position.x > path_maximum)) current_speed.x -= current_speed.x;

	//pathfind
	PathfindtoPlayer(400);

	//state machine
	switch (state)
	{
		run = idle;
	case IDLE:
		current_animation = &idle;
		break;
	case JUMP:
		current_animation = &jump;
		break;
	case RUN_FORWARD:
		//current_animation = &run;
		current_speed.x = speed.x * 2;
		flip = SDL_FLIP_NONE;
		break;
	case RUN_BACKWARD:
		//current_animation = &run;
		current_speed.x = -speed.x;
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case FALL:
		current_animation = &fall;
		break;
	case ATTACK:
		current_animation = &attack;
		break;
	case DIE:
		current_animation = &die;
		break;
	default:
		break;
	}

	//Movement Control
	if (!grounded) {
		//if (current_speed.y > max_falling_speed) 
		current_speed.y -= gravity * dt;
		position.y -= current_speed.y * dt;
	}

	position.x += current_speed.x * dt;

	//collider control

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);
	if ((raycast != nullptr) && (collider != nullptr))
		raycast->SetPos(collider->rect.x + collider->rect.w * 0.5f - raycast->rect.w * 0.5f, position.y + current_animation->GetCurrentFrame().h);

	return ret;
}
*/

/*
bool j1FlyingEnemy::PostUpdate() {
	bool ret = true;
	App->render->Blit(texture, position.x, position.y, &current_animation->GetCurrentFrame(), flip);
	return ret;
}
*/

/*
void j1FlyingEnemy::PathfindtoPlayer(int range) {

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
*/

/*
void j1FlyingEnemy::OnCollision(Collider* c1, Collider* c2) {

	if (c1 == raycast)
	{
		last_collider = c2;
	}

	switch (c2->type)
	{
	case COLLIDER_WALL:
		if (position.y + current_animation->GetCurrentFrame().h < c2->rect.y + COLLIDER_MARGIN)
		{
			grounded = true;
			position.y = c2->rect.y - current_animation->GetCurrentFrame().h;
			current_speed.y = 0;
			state = IDLE;
		}

		if (position.y + current_animation->GetCurrentFrame().h > c2->rect.y) {
			position.x = lastPosition.x;
		}
		if (position.y > c2->rect.y + c2->rect.h - COLLIDER_MARGIN)
		{
			position.y = c2->rect.y + c2->rect.h;
			current_speed.y = 0;
		}
		if ((position.y > c2->rect.y) && (position.x > c2->rect.x) && (position.x + current_animation->GetCurrentFrame().w < c2->rect.x + c2->rect.w) && (position.x < c2->rect.x + c2->rect.w))
		{
			position.y = lastPosition.y;
			if (lastPosition.y + current_animation->GetCurrentFrame().h > c2->rect.y) {
				//position.y = c2->rect.y - current_animation->GetCurrentFrame().h;
			}
		}
		break;
	case COLLIDER_PLAYER:
		//App->entities->DestroyEntity(this);
		//state = DIE;
		break;
	default:
		break;
	}
}
*/


