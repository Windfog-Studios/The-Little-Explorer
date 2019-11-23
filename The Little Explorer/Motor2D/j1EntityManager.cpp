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
#include "brofiler/Brofiler/Brofiler.h"


j1EntityManager::j1EntityManager(){

	name.create("entityManager");

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
	case EntityType::TRAP:
		break;
	case EntityType::PARTICLES:
		break;
	case EntityType::UNKNOWN:
		break;
	default:
		break;
	}

	if ((entity != nullptr)&&(entity != player)) entities.add(entity);

	return entity;
}

void j1EntityManager::DestroyEntity(j1Entity* delete_entity)
{
	RELEASE(delete_entity);
}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;

	player = (j1Player*)App->entities->CreateEntity(EntityType::PLAYER, App->scene->player_x_position, App->scene->player_y_position);

	return ret;
}

bool j1EntityManager::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntitiesUpdate", Profiler::Color::MediumPurple)
	bool ret = true;

	for (p2List_item<j1Entity*>* entity = entities.start; entity != nullptr; entity = entity->next)
	{
		if (entity->data != player)
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
		if (entity->data != player)
		{
			entity->data->PostUpdate();
		}
	}
	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;
	App->tex->UnLoad(walking_enemy_tex);
	walking_enemy_tex = nullptr;

	for (p2List_item<j1Entity*>* entity = entities.start; entity != nullptr; entity = entity->next)
	{
		entity->data->DestroyEntity(entity->data);
	}

	return ret;
}

void j1EntityManager::OnCollision(Collider* c1, Collider* c2){
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
