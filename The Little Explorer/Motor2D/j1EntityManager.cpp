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


j1EntityManager::j1EntityManager()
{
	name.create("entityManager");
}


j1EntityManager::~j1EntityManager()
{}
/*
bool j1EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;


	return ret;
}

bool j1EntityManager::Start()
{
	return true;
}

bool j1EntityManager::PreUpdate()
{

}

bool j1EntityManager::Update(float dt)
{

}

bool j1EntityManager::PostUpdate()
{

}

bool j1EntityManager::CleanUp()
{

}

void j1EntityManager::OnCollision(Collider* c1, Collider* c2)
{

}

bool j1EntityManager::Load(pugi::xml_node& data)
{
}

bool j1EntityManager::Save(pugi::xml_node& data)
{
}
*/

Entity* j1EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr;

	return entity;
}

void j1EntityManager::DestroyEntity(Entity* delete_entity)
{

}
