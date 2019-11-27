#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1UI.h"
#include "j1Particles.h"
#include "j1WalkingEnemy.h"
#include "j1FlyingEnemy.h"
#include "j1Trap.h"
#include "brofiler/Brofiler/Brofiler.h"


j1EntityManager::j1EntityManager(){
	name.create("entities");
}


j1EntityManager::~j1EntityManager(){
}

j1Entity* j1EntityManager::CreateEntity(EntityType type, int position_x, int position_y)
{
	//static_assert(EntityType::UNKNOWN == 4, "code needs update");
	j1Entity* entity = nullptr;
	switch (type)
	{
	case EntityType::PLAYER:
		entity = new j1Player();
		break;
	case EntityType::WALKING_ENEMY:
		entity = new j1WalkingEnemy();
		entity->position.x = position_x;
		entity->position.y = position_y;
		break;
	case EntityType::FLYING_ENEMY:
		entity = new j1FlyingEnemy();
		entity->position.x = position_x;
		entity->position.y = position_y;
		break;
	case EntityType::TRAP:
		entity = new j1Trap();
		entity->position.x = position_x;
		entity->position.y = position_y;
		break;
	case EntityType::PARTICLES:
		break;
	case EntityType::UNKNOWN:
		break;
	default:
		break;
	}

	if (entity != nullptr) entities.add(entity);

	return entity;
}

void j1EntityManager::DestroyEntity(j1Entity* entity)
{
	p2List_item<j1Entity*>* item;

	if (entity != nullptr) {
		item = entities.At(entities.find(entity));
		if (entity->collider != nullptr)
		{
			entity->collider->to_delete = true;
			entity->collider = nullptr;
		}
		if (entity->raycast != nullptr) {
			entity->raycast->to_delete = true;
			entity->raycast = nullptr;
		}
		entities.del(item);;
	}
}


bool j1EntityManager::Awake(pugi::xml_node& config){
	bool ret = true;

	config_data = config;

	player = new j1Player();
	player->Awake(config.child("player"));
	entities.add(player);

	gravity = config.child("gravity").attribute("value").as_int();

	//load walking enemy data
	walking_enemy_speed = config.child("walking_enemy").child("running_speed").attribute("value").as_int();
	walking_enemy_health = config.child("walking_enemy").child("health").attribute("value").as_int();
	walking_enemy_damage = config.child("walking_enemy").child("damage").attribute("value").as_int();

	//flying enemy data
	flying_enemy_speed = config.child("flying_enemy").child("flying_speed").attribute("value").as_int();
	flying_enemy_health = config.child("flying_enemy").child("health").attribute("value").as_int();
	flying_enemy_damage = config.child("flying_enemy").child("damage").attribute("value").as_int();
	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;
	player->Start();
	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;
	App->tex->UnLoad(walking_enemy_texture);
	walking_enemy_texture = nullptr;

	App->tex->UnLoad(flying_enemy_texture);
	flying_enemy_texture = nullptr;

	App->tex->UnLoad(trap_texture);
	trap_texture = nullptr;

	for (p2List_item<j1Entity*>* entity = entities.start; entity != nullptr; entity = entity->next)
	{
		entity->data->DestroyEntity(entity->data);
	}

	return ret;
}

bool j1EntityManager::PreUpdate()
{
	bool ret = true;
	player->PreUpdate();
	return ret;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntitiesUpdate", Profiler::Color::MediumPurple)
	bool ret = true;

	for (p2List_item<j1Entity*>* entity = entities.start; entity != nullptr; entity = entity->next)
	{
		if (entity != nullptr)
		{
			entity->data->Update(dt);
		}
	}

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("EntitiesPostUpdate", Profiler::Color::Khaki)
	bool ret = true;
	for (p2List_item<j1Entity*>* entity = entities.start; entity != nullptr; entity = entity->next)
	{
		entity->data->PostUpdate();
	}
	return ret;
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	bool ret = true;

	return ret;
}

bool j1EntityManager::Save(pugi::xml_node& data)
{
	bool ret = true;

	return ret;
}

void j1EntityManager::LoadTextures() {
	walking_enemy_texture = App->tex->Load("sprites/characters/Enemies/knight_spritesheet.png");
	flying_enemy_texture = App->tex->Load("sprites/characters/Sprite_bat.png");
	trap_texture = App->tex->Load("sprites/characters/Plant bite anim.png");
}
