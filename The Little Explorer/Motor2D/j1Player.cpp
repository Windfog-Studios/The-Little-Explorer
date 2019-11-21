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
#include "j1EntityManager.h"

j1Player::j1Player():j1Entity (EntityType::PLAYER) {

	name.create("player");
	LoadAnimations();

}

j1Player::~j1Player(){ }

bool j1Player::Awake(pugi::xml_node& config) {

	bool ret = true;
	LOG("Loading Player Data");

	folder.create(config.child("folder").child_value());

	current_animation = &idle;

	//set initial attributes of the player
	max_running_speed = config.child("max_running_speed").attribute("value").as_float();
	side_speed = config.child("side_speed").attribute("value").as_float();
	acceleration = config.child("acceleration").attribute("value").as_float();
	deceleration = config.child("deceleration").attribute("value").as_float();

	jumpImpulse = config.child("jumpImpulse").attribute("value").as_float();
	doubleJumpImpulse = config.child("doubleJumpImpulse").attribute("value").as_float();

	gravity = config.child("gravity").attribute("value").as_float();

	//player fx
	die_fx_path = config.child("dieFX").attribute("source").as_string();
	jump_fx_path = config.child("jumpFX").attribute("source").as_string();

	//colliders
	collider = App->collision->AddCollider(current_animation->GetCurrentFrame(), COLLIDER_PLAYER, (j1Module*)App->player); //a collider to start
	raycast = App->collision->AddCollider(SDL_Rect{ 0,0,20,5 }, COLLIDER_PLAYER, (j1Module*)App->player);

	return ret;
}

bool j1Player::Start(){

	position.x = initial_x_position = App->scene->player_x_position;
	position.y = initial_x_position = App->scene->player_y_position;

	texture = App->tex->Load("sprites/characters/spritesheet_traveler.png");
	die_fx = App->audio->LoadFx(die_fx_path.GetString());
	jump_fx = App->audio->LoadFx(jump_fx_path.GetString());

	return true;
}

bool j1Player::CleanUp() {
	collider->to_delete = true;
	collider = nullptr;
	App->tex->UnLoad(texture);
	texture = nullptr;
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
			current_speed.y = 0;
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
					current_speed.y = jumpImpulse;
					grounded = false;
				}

			}

			if (state == RUN_FORWARD)
			{
				if (player_input.pressing_D)
				{
					if (current_speed.x < max_running_speed)
					{
						current_speed.x += acceleration;
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
					current_speed.y = jumpImpulse;
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
					if (current_speed.x > -max_running_speed)
					{
						current_speed.x -= acceleration;
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
					current_speed.y = jumpImpulse;
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
				if ((player_input.pressing_D)&&(current_speed.x < side_speed)) current_speed.x += acceleration;
				if ((player_input.pressing_A)&&(current_speed.x > -side_speed)) current_speed.x -= acceleration;

				//double jump
				if ((player_input.pressing_space) && (can_double_jump == true) && (current_speed.y <= jumpImpulse * 0.5f))
				{
					App->audio->PlayFx(jump_fx);
					jump.Reset();
					current_speed.y = doubleJumpImpulse;
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

				if ((player_input.pressing_D) && (can_go_right == true)) current_speed.x += side_speed * 0.25;
				if ((player_input.pressing_A) && (can_go_left == true)) current_speed.x -= side_speed * 0.25;

				if ((player_input.pressing_space) && (can_double_jump == true) & (current_speed.y <= jumpImpulse * 0.5f))
				{
					jump.Reset();
					state = JUMP;
					App->audio->PlayFx(jump_fx);
					current_speed.y = doubleJumpImpulse;
					can_double_jump = false;
					grounded = false;
					App->particles->AddParticle(App->particles->dust, position.x, position.y + current_animation->GetCurrentFrame().h * 0.75f, COLLIDER_NONE, 0, flip);
				}

				if (current_animation->Finished())
				{
					fall.Reset();
				}
			}
		}
	}

	if (god)
	{
		if (player_input.pressing_W) position.y -= current_speed.x;
		if (player_input.pressing_S) position.y += current_speed.x;
	}
	return true;
}

bool j1Player::Update(float dt){

	MovementControl(dt); //calculate new position

	collider->SetPos(floor(position.x), position.y);
	raycast->SetPos(floor(collider->rect.x + collider->rect.w * 0.5f - raycast->rect.w * 0.5f), collider->rect.y + collider->rect.h);

	if (last_collider != nullptr)
	{
		if (!raycast->CheckCollision(last_collider->rect))
		{
			grounded = false;
		}
	}

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
		if (current_speed.y <= 0)
		{
			state = FALL;
			jump.Reset();
		}
		if ((last_state = RUN_FORWARD)||(last_state == RUN_BACKWARD))
		{
			current_speed.x *= 0.5f;
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
		App->render->Blit(texture, position.x, position.y, &current_animation->GetCurrentFrame(), flip);
	}
	return true;	
}

void j1Player::MovementControl(float dt) {

	if (!grounded){
		if (!god) current_speed.y -= gravity;
		position.y -= current_speed.y * dt;
	}

	//deceleration
	if ((!player_input.pressing_D) && (current_speed.x > 0)) current_speed.x -= deceleration * dt;
	if ((!player_input.pressing_A) && (current_speed.x < 0)) current_speed.x += deceleration * dt;
	if ((floor(current_speed.x)<=20)&&(floor(current_speed.x) >= -20))
		current_speed.x = 0;


	position.x += current_speed.x * dt;

	LOG("Speed x: %.2f y: %.2f", current_speed.x, current_speed.y);
	LOG("Floor Speed x: %.2f y: %.2f", floor(current_speed.x), floor(current_speed.y));

	//LOG("Grounded %i", grounded);
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	if (c1 == raycast)
	{
		last_collider = c2;
	}

	if (!god)
	{
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
				
				if (position.y +current_animation->GetCurrentFrame().h > c2->rect.y) {
					position.x = lastPosition.x;
				}
				if (position.y > c2->rect.y + c2->rect.h - COLLIDER_MARGIN)
				{
					position.y = c2->rect.y + c2->rect.h;
					current_speed.y = 0;
				}
				break;
			case COLLIDER_DEATH:
				if (!god) {
					current_speed.x = 0;
					
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
				/*
				if (((position.y + current_animation->GetCurrentFrame().h >= c2->rect.y)&&(lastPosition.y + current_animation->GetCurrentFrame().h <= c2->rect.y + COLLIDER_MARGIN))||(grounded == true))
				{
					position = lastPosition;
					current_speed.x = current_speed.y = 0;
					grounded = true;
					if (last_state == FALL)
					{
						state = IDLE;
					}
				}*/
				if (c1->rect.y + c1->rect.h * 0.5f < c2->rect.y)
				{
					grounded = true;
					position.y = c2->rect.y - current_animation->GetCurrentFrame().h;
					current_speed.y = 0;
					state = IDLE;
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

