#ifndef __j1PARTICLES_H__
#define __j1PARTICLES_H__

#include "j1Module.h"
#include "Animation.h"

#include "p2Point.h"
#include "j1Collision.h"
#include "SDL/include/SDL_render.h"

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

class j1Particles : public j1Module
{
public:
	j1Particles();
	~j1Particles();

	bool Start();
	bool Update();
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

#endif // __j1PARTICLES_H__