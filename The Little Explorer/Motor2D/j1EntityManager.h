#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "SDL_image/include/SDL_image.h"

class j1Entity;
struct SDL_Texture;


enum class EntityType
{
	PLAYER,
	WALKING_ENEMY,
	FLYING_ENEMY,
	TRAP,
	PARTICLES,
	UNKNOWN
};


class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	virtual bool Awake(pugi::xml_node&);
	virtual bool Start();

	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();

	virtual bool CleanUp();

	virtual bool Load(pugi::xml_node& data);
	virtual bool Save(pugi::xml_node& data);

	j1Entity* CreateEntity(EntityType type, int position_x, int position_y);
	virtual void DestroyEntity(j1Entity* delete_entity);
	void DestroyAllEntities();

	void LoadEnemiesInfo();

public:

	p2List<j1Entity*> entities;
	pugi::xml_node config_data;

	j1Player* player;

public:
	int gravity = 0;

	//speed
	int walking_enemy_speed = 0;
	int flying_enemy_speed;

	//health
	int walking_enemy_health = 0;
	int flying_enemy_health = 0;
	int trap_health = 0;

	//damage
	int walking_enemy_damage = 0;
	int flying_enemy_damage = 0;
	int trap_damage = 0;

	//textures
	SDL_Texture* walking_enemy_texture = nullptr;
	SDL_Texture* flying_enemy_texture = nullptr;
	SDL_Texture* trap_texture = nullptr;

	//sfx
	uint walking_enemy_attack_fx;
	uint flying_enemy_attack_fx;

	uint walking_enemy_die_fx;
	uint flying_enemy_die_fx;

	//animations
	p2List<Animation*> walking_enemy_animations;
	p2List<Animation*> flying_enemy_animations;

};

#endif // !_ENTITY_MANAGER_H_

