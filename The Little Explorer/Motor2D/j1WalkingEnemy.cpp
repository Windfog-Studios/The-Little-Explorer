#include "j1WalkingEnemy.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

j1WalkingEnemy::j1WalkingEnemy() :j1Entity(EntityType::WALKING_ENEMY) {
	name.create("walking_enemy");
	texture = App->tex->Load("sprites/characters/Enemies/knight_spritesheet.png");
	current_animation = &idle;
	idle.PushBack({ 16,34,27,30 });
	collider = App->collision->AddCollider({ 16,34,27,30 },COLLIDER_ENEMY,(j1Module*)this);
	raycast = App->collision->AddCollider({ 16,34,20,5 }, COLLIDER_ENEMY, (j1Module*)this);
	lastPosition = position;
	player = App->entities->player;
	speed.x = 20;
	health = 50;
	flip = SDL_FLIP_HORIZONTAL;
}

j1WalkingEnemy::~j1WalkingEnemy() {
	App->entities->DestroyEntity(this);
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
	PathfindtoPlayer(400, player);

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
	if ((raycast != nullptr)&&(collider != nullptr)) 
		raycast->SetPos(collider->rect.x + collider->rect.w * 0.5f - raycast->rect.w * 0.5f, position.y + current_animation->GetCurrentFrame().h);

	return ret;
}

bool j1WalkingEnemy::PostUpdate() {
	bool ret = true;
	App->render->Blit(texture, position.x, position.y, &current_animation->GetCurrentFrame(), flip);
	return ret;
}


void j1WalkingEnemy::OnCollision(Collider* c1, Collider* c2) {

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