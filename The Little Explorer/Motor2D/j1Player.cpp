#include "j1Player.h"
#include "p2Log.h"
#include "j1App.h"
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

j1Player::j1Player():j1Module () {

	name.create("player");

	LoadAnimations();

}

j1Player::~j1Player(){ }

bool j1Player::Awake(pugi::xml_node& config) {

	LOG("Loading Player Data");
	bool ret = true;

	current_animation = &idle;

	folder.create(config.child("folder").child_value());

	//set initial attributes of the player
	max_running_speed = config.child("running_speed").attribute("value").as_float();
	acceleration = config.child("acceleration").attribute("value").as_float();
	jumpImpulse = config.child("jumpImpulse").attribute("value").as_float();
	doubleJumpImpulse = config.child("doubleJumpImpulse").attribute("value").as_float();
	gravity = config.child("gravity").attribute("value").as_float();
	side_speed = config.child("side_speed").attribute("value").as_float();

	//player fx
	die_fx_path = config.child("dieFX").attribute("source").as_string();
	jump_fx_path = config.child("jumpFX").attribute("source").as_string();

	collider = App->collision->AddCollider(current_animation->GetCurrentFrame(), COLLIDER_PLAYER, (j1Module*)App->player); //a collider to start
	raycast = App->collision->AddCollider(SDL_Rect{ 0,0,20,5 }, COLLIDER_PLAYER, (j1Module*)App->player);

	return ret;
}

bool j1Player::Start(){
	//load character sprites
	player_tex = App->tex->Load("sprites/characters/spritesheet_traveler.png");
	position.x = initial_x_position = App->scene->player_x_position;
	position.y = initial_x_position = App->scene->player_y_position;

	die_fx = App->audio->LoadFx(die_fx_path.GetString());
	jump_fx = App->audio->LoadFx(jump_fx_path.GetString());

	return true;
}

bool j1Player::CleanUp() {
	collider->to_delete = true;
	collider = nullptr;
	App->tex->UnLoad(player_tex);
	player_tex = nullptr;
	return true;
}


bool j1Player::PreUpdate(){
	//get player input
	player_input.pressing_W = App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
	player_input.pressing_A = App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	player_input.pressing_S = App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	player_input.pressing_D = App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
	player_input.pressing_F = App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT;
	player_input.pressing_space = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN;

	lastPosition = position;
	last_state = state;

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		god = !god;
		if (god)
		{
			state = IDLE;
			speed.y = 0;
		}
	}

	if (!App->pause)
	{
		//speed.x = 0;
		if(!controls_blocked){
			if (state == IDLE)
			{
				can_double_jump = true;

				if (player_input.pressing_D)
				{
					state = RUN_FORWARD;
				}

				if (player_input.pressing_A)
				{
					state = RUN_BACKWARD;
				}

				if (player_input.pressing_S)
				{
					state = CROUCH_DOWN;
				}

				if (player_input.pressing_F)
				{
					if (flip == SDL_FLIP_NONE)
					{
						state = SLIDE_FORWARD;
					}
					else
					{
						state = SLIDE_BACKWARD;
					}
				}

				if ((player_input.pressing_space) && (!god))
				{
					App->audio->PlayFx(jump_fx);
					state = JUMP;
					speed.y = jumpImpulse;
					grounded = false;
				}

			}

			if (state == RUN_FORWARD)
			{
				if (player_input.pressing_D)
				{
					if (speed.x < max_running_speed)
					{
						speed.x += acceleration;
					}
				}

				if (!player_input.pressing_D)
				{
					state = IDLE;
				}

				if ((player_input.pressing_space) && (!god))
				{
					App->audio->PlayFx(jump_fx);
					state = JUMP;
					speed.y = jumpImpulse;
					grounded = false;
				}

				if (player_input.pressing_F)
				{
					state = SLIDE_FORWARD;
				}
			}

			if (state == RUN_BACKWARD)
			{
				if (player_input.pressing_A)
				{
					if (speed.x > -max_running_speed)
					{
						speed.x -= acceleration;
					}
				}

				if (!player_input.pressing_A)
				{
					state = IDLE;
				}

				if ((player_input.pressing_space)&&(!god))
				{
					App->audio->PlayFx(jump_fx);
					state = JUMP;
					speed.y = jumpImpulse;
					grounded = false;
				}

				if (player_input.pressing_F)
				{
					state = SLIDE_BACKWARD;
				}
			}


			if (state == CROUCH_DOWN)
			{
				if (!player_input.pressing_S)
				{
					state = CROUCH_UP;
					crouch_down.Reset();
				}
			}

			if (state == CROUCH_UP)
			{
				if (current_animation->Finished()) {
					state = IDLE;
					crouch_up.Reset();
				}
			}

			if (state == SLIDE_FORWARD)
			{
				if (!player_input.pressing_F)
				{
					state = IDLE;
				}
			}

			if (state == SLIDE_BACKWARD)
			{
				if (!player_input.pressing_F)
				{
					state = IDLE;
				}
			}

			if (state == JUMP)
			{
				if (player_input.pressing_D) position.x += side_speed;
				if (player_input.pressing_A) position.x -= side_speed;

				//double jump
				if ((player_input.pressing_space) && (can_double_jump == true) && (speed.y <= jumpImpulse * 0.5f))
				{
					App->audio->PlayFx(jump_fx);
					jump.Reset();
					speed.y = doubleJumpImpulse;
					can_double_jump = false;
					App->particles->AddParticle(App->particles->dust, position.x, position.y + current_animation->GetCurrentFrame().h * 0.75f, COLLIDER_NONE, 0, flip);
				}

				if (current_animation->Finished())
				{
					state = FALL;
					jump.Reset();
				}

			}
			if (state == FALL)
			{
				//let the player move while faling
				if ((player_input.pressing_D) && (can_go_right == true)) position.x += side_speed;
				if ((player_input.pressing_A) && (can_go_left == true)) position.x -= side_speed;

				//double jump
				if ((player_input.pressing_space) && (can_double_jump == true) & (speed.y <= jumpImpulse * 0.5f))
				{
					jump.Reset();
					state = JUMP;
					App->audio->PlayFx(jump_fx);
					speed.y = doubleJumpImpulse;
					can_double_jump = false;
					grounded = false;
					App->particles->AddParticle(App->particles->dust, position.x, position.y + current_animation->GetCurrentFrame().h * 0.75f, COLLIDER_NONE, 0, flip);
				}

				if (current_animation->Finished())
				{
					fall.Reset();
				}
			}

			/*if ((speed.y < 3) && ((state == IDLE) ||(state == RUN_FORWARD) || (state == RUN_BACKWARD) ) )
			{
				state = FALL;
			}*/
		}
	}

	if (god)
	{
		if (player_input.pressing_W) position.y -= speed.x;
		if (player_input.pressing_S) position.y += speed.x;
	}
	return true;
}

bool j1Player::Update(float dt){

	MovementControl(dt); //calculate new position

	collider->SetPos(floor(position.x), floor(position.y));
	raycast->SetPos(floor(position.x + current_animation->GetCurrentFrame().w * 0.25f), floor(position.y + current_animation->GetCurrentFrame().h));

	switch (state)
	{
	case IDLE:
		current_animation = &idle;
		break;

	case RUN_FORWARD:
		current_animation = &run;
		flip = SDL_FLIP_NONE;
		break;

	case RUN_BACKWARD:
		current_animation = &run;
		flip = SDL_FLIP_HORIZONTAL;
		break;

	case CROUCH_DOWN:
		current_animation = &crouch_down;
		break;

	case CROUCH_UP:
		current_animation = &crouch_up;
		break;

	case SLIDE_FORWARD:
		current_animation = &slide;
		flip = SDL_FLIP_NONE;
		break;

	case SLIDE_BACKWARD:
		current_animation = &slide;
		flip = SDL_FLIP_HORIZONTAL;
		break;

	case JUMP:
		current_animation = &jump;
		if (speed.y <= 0)
		{
			state = FALL;
			jump.Reset();
		}
		if ((last_state = RUN_FORWARD)||(last_state == RUN_BACKWARD))
		{
			speed.x *= 0.5f;
		}
		break;
	case FALL:
		current_animation = &fall;
		break;
	default:
		LOG("No state found");
		break;
	}

	return true;
}

bool j1Player::PostUpdate() {

	if (visible)
	{
		App->render->Blit(player_tex, position.x, position.y, &current_animation->GetCurrentFrame(), flip);
	}

	return true;	
}

void j1Player::MovementControl(float dt) {
	dt = floor(dt) * 0.1;
	//speed.x *= ceil(dt);
	//speed.y *= ceil(dt);
	if (!god) speed.y -= gravity;
	if ((!player_input.pressing_D) && (speed.x > 0)) speed.x -= acceleration;
	if ((!player_input.pressing_A) && (speed.x < 0)) speed.x += acceleration * 1.5f;
	position.x += speed.x * dt;
	if (grounded == false)
	{
		position.y -= speed.y * dt;
	}
	//LOG("velocity x. %.2f y: %.2f", velocity.x, velocity.y);
	//LOG("Speed x: %.2f", speed.x);
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	if (!god)
	{
		switch (c2->type)
		{
			case COLLIDER_WALL:
				//position = lastPosition;
				//speed.y = 0;
				if (position.y + current_animation->GetCurrentFrame().h < c2->rect.y + COLLIDER_MARGIN)
				{
					grounded = true;
					position.y = c2->rect.y - current_animation->GetCurrentFrame().h;
					speed.y = 0;
					state = IDLE;
				}
				/*
				if (position.y > c2->rect.y + COLLIDER_MARGIN) {
					position.x = lastPosition.x;
				}*/
				break;
			case COLLIDER_DEATH:
				if (!god) {
					speed.x = 0;
					
					if (App->ui->transition == false)
					{
						App->audio->PlayFx(die_fx);
						App->ui->transition = true;
						App->scene->blocked_camera = true;
						App->ui->ResetTransition();
						App->scene->Reset_Camera(1);
						controls_blocked = true;
					}
				}
				break;
			case COLLIDER_PLATFORM:
				if (((position.y + current_animation->GetCurrentFrame().h >= c2->rect.y)&&(lastPosition.y + current_animation->GetCurrentFrame().h <= c2->rect.y + COLLIDER_MARGIN))||(grounded == true))
				{
					position = lastPosition;
					speed.x = speed.y = 0;
					grounded = true;
					if (last_state == FALL)
					{
						state = IDLE;
					}
				}
				break;
			case COLLIDER_CHANGE_LEVEL:
				if (App->ui->transition == false)
				{
					if (App->scene->current_level == LEVEL_1) App->scene->want_to_load = LEVEL_2;
					if (App->scene->current_level == LEVEL_2) App->scene->want_to_load = LEVEL_1;
					App->ui->transition = true;
					App->ui->loaded = false;
					App->ui->transition_moment = SDL_GetTicks();
					App->ui->ResetTransition();
					controls_blocked = true;
				}
				break;
			default:
				break;
		}
	}
}

bool j1Player::LoadAnimations() {
	pugi::xml_parse_result result = animation_doc.load_file("sprites/characters/animations.xml");
	bool ret = true;
	uint i = 0u;
	uint j = 0;
	
	if (result == NULL)
	{
		LOG("Could not load animations xml file %s. pugi error: %s", "animations.xml", result.description());
		ret = false;
	}

	animations.add(&idle);
	animations.add(&run);
	animations.add(&crouch_down);
	animations.add(&crouch_up);
	animations.add(&jump);
	animations.add(&fall);

	pugi::xml_node animation = animation_doc.child("animations").child("animation");
	pugi::xml_node frame;
	p2List_item<Animation*>* item = animations.start;
	int x, y, w, h;
	float anim_speed = 1;

	LOG("Loading animations ---------");

	for (animation ; animation; animation = animation.next_sibling("animation"))
	{
		item->data->loop = animation.attribute("loop").as_bool();

		for (frame = animation.child("data").child("frame"); frame; frame = frame.next_sibling("frame"))
		{
			x = frame.attribute("x").as_int();
			y = frame.attribute("y").as_int();
			w = frame.attribute("w").as_int();
			h = frame.attribute("h").as_int();
			anim_speed = frame.attribute("speed").as_float();

			item->data->PushBack({x,y,w,h}, anim_speed);
		}
		i++;
		item = item->next;
	}

	LOG("%u animations loaded", i);

	return ret;
}

bool j1Player::Save(pugi::xml_node& data) const {

	pugi::xml_node p_position = data.append_child("position");

	p_position.append_attribute("x") = position.x;
	p_position.append_attribute("y") = position.y;
	return true;
}

bool j1Player::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();

	return true;
}

