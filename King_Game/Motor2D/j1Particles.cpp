#include <math.h>
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Particles.h"
#include "j1Player.h"

#include "SDL/include/SDL_timer.h"

j1Particles::j1Particles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	active[i] = nullptr;

}

j1Particles::~j1Particles()
{}

// Load assets
bool j1Particles::Start()
{
	LOG("Loading particles");

	return true;
}

// Unload assets
bool j1Particles::CleanUp()
{
	LOG("Unloading particles");

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}

	return true;
}

// Update: draw background
bool j1Particles::Update()
{
	bool ret = true;
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if (p == nullptr)
			continue;

		if (p->Update() == false)
		{
			delete p;
			active[i] = nullptr;
		}
		else if (SDL_GetTicks() >= p->born)
		{
			
			if (p->coll != nullptr)
			{
				/*
				if ((p->coll->type == COLLIDER_PLAYER_PARTICLES) || (p->coll->type == COLLIDER_PLAYER_2_PARTICLES))
				{
					particle_tex = fire_sword_tex;
				}
				*/
			}
			else 
			{
				particle_tex = fx_particles;
			}
			if (p->flip == SDL_FLIP_NONE)
			{
				App->render->Blit(particle_tex, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));
			}
			else
			{
				App->render->Blit(particle_tex, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()), SDL_FLIP_HORIZONTAL);
			}
			if (p->fx_played == false)
			{
				p->fx_played = true;
				// play the audio SFX
			}
		}
	}


	return ret;
}

Particle* j1Particles::AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type, Uint32 delay, SDL_RendererFlip flip)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			if (collider_type != COLLIDER_NONE)
				p->coll = App->collision->AddCollider(p->anim.GetCurrentFrame(), collider_type, this);
			active[i] = p;
			p->flip = flip;
			return p;
		}
	}

}

void j1Particles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr && active[i]->coll == c1)
		{
			delete active[i];
			active[i] = nullptr;
			break;
		}
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed),
	fx(p.fx), born(p.born), life(p.life)
{}

Particle::~Particle()
{
	if (coll != nullptr) {
		coll->to_delete = true;
		coll = nullptr;
	}

}

bool Particle::Update()
{
	bool ret = true;

	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
			ret = false;
	}
	else
		if (anim.Finished())
			ret = false;

	if (!App->pause)
	{
		if (coll != nullptr)
		{
			/*
			if ((coll->type == COLLIDER_PLAYER_PARTICLES) || (coll->type == COLLIDER_PLAYER_2_PARTICLES)) {
				if (flip == SDL_FLIP_NONE)
				{
					position.x += speed.x;
					position.y += speed.y;
				}
				else if (flip == SDL_FLIP_HORIZONTAL)
				{
					position.x -= speed.x;
					position.y += speed.y;
				}
			}


			if (coll != nullptr)
				coll->SetPos(position.x, position.y);

			if (position.y > 160) 
				ret = false;
				*/
		}
		return ret;
	}
}

