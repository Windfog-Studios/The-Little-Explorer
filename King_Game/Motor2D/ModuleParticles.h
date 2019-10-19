#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleCollision.h"
#include "SDL/include/SDL_render.h"

#define BULLET_LIFE 500
#define BULLET_ENEMY_LIFE 2000
#define BULLET_SPEED 10
#define BULLET_ENEMY_SPEED 2

#define MAX_ACTIVE_PARTICLES 100

struct SDL_Texture;
struct Collider;
enum COLLIDER_TYPE;

struct Particle
{
	Collider* coll = nullptr;
	Animation anim;
	uint fx = 0;
	fPoint position;
	fPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	bool fx_played = false;
	SDL_RendererFlip flip;

	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
};

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	Particle* AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type = COLLIDER_NONE, Uint32 delay = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	SDL_Texture* particle_tex = nullptr;
	SDL_Texture* tornado_tex = nullptr;
	SDL_Texture* fire_sword_tex = nullptr;
	SDL_Texture* fx_particles = nullptr;
	Particle* active[MAX_ACTIVE_PARTICLES];

public:
	Particle tornado;
	Particle fire_sword;
	Particle blood;
	Particle flash;
	Particle dust;

};

#endif // __MODULEPARTICLES_H__