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
	{
		//idle animation
		/*
		idle.PushBack({ 21, 226, 32, 56 }, 0.05f);
		idle.PushBack({ 71, 226, 32, 56 }, 0.05f);
		idle.PushBack({ 121, 226, 32, 56 }, 0.05f);
		idle.PushBack({ 171, 226, 32, 56 }, 0.05f);
		idle.PushBack({ 221, 226, 32, 56 }, 0.05f);
		idle.PushBack({ 271, 226, 32, 56 }, 0.05f);
		idle.PushBack({ 321, 226, 32, 56 }, 0.05f);
		idle.PushBack({ 369, 226, 32, 56 }, 0.05f);
		idle.loop = true;

		//walk animation
		walk.PushBack({ 21, 31, 33, 56 }, 0.2f);
		walk.PushBack({ 70, 31, 32, 56 }, 0.2f);
		walk.PushBack({ 119, 31, 30, 56 }, 0.2f);
		walk.PushBack({ 169, 31, 30, 56 }, 0.2f);
		walk.PushBack({ 215, 31, 30, 56 }, 0.2f);
		walk.PushBack({ 264, 31, 30, 56 }, 0.2f);
		walk.PushBack({ 312, 31, 30, 56 }, 0.2f);
		walk.PushBack({ 359, 31, 30, 56 }, 0.2f);
		walk.loop = false;

		//run animation
		run.PushBack({ 21, 156, 46, 53 }, 0.1f);
		run.PushBack({ 79, 157, 44, 52 }, 0.1f);
		run.PushBack({ 136, 155, 41, 54 }, 0.1f);
		run.PushBack({ 196, 155, 46, 54 }, 0.1f);
		run.PushBack({ 260, 157, 42, 52 }, 0.1f);
		run.PushBack({ 322, 155, 40, 54 }, 0.1f);
		run.loop = true;

		//crouch down animation
		crouch_down.PushBack({ 21, 302, 32, 56 }, 0.2f);
		crouch_down.PushBack({ 75, 302, 35, 56 }, 0.2f);
		crouch_down.PushBack({ 125, 302, 43, 56 }, 0.2f);
		crouch_down.PushBack({ 184, 302, 43, 56 }, 0.2f);
		crouch_down.loop = false;

		//crouch up animation
		crouch_up.PushBack({ 184, 302, 43, 56 }, 0.2f);
		crouch_up.PushBack({ 125, 302, 43, 56 }, 0.2f);
		crouch_up.PushBack({ 75, 302, 35, 56 }, 0.2f);
		crouch_up.PushBack({ 21, 302, 32, 56 }, 0.2f);
		crouch_up.loop = false;

		//slide animation
		slide.PushBack({ 21, 91, 52, 52 }, 0.2f);
		slide.PushBack({ 86, 91, 52, 52 }, 0.2f);
		slide.loop = true;

		//jump animation
		jump.PushBack({ 420, 315, 49, 43 }, 0.2f);
		jump.PushBack({ 477, 316, 52, 42 }, 0.2f);
		jump.PushBack({ 539, 294, 32, 64 }, 0.2f);
		jump.PushBack({ 582, 294, 32, 64 }, 0.2f);
		jump.PushBack({ 626, 294, 32, 64 }, 0.2f);
		jump.PushBack({ 669, 294, 32, 64 }, 0.2f);
		jump.PushBack({ 710, 294, 32, 64 }, 0.2f);
		jump.PushBack({ 752, 294, 32, 64 }, 0.2f);
		jump.PushBack({ 796, 294, 32, 64 }, 0.2f);
		jump.loop = false;


		fall.PushBack({ 37, 379, 32, 64 }, 0.2f);
		fall.PushBack({ 78, 383, 37, 60 }, 0.2f);
		fall.PushBack({ 124, 384, 47, 59 }, 0.2f);
		fall.PushBack({ 183, 385, 51, 58 }, 0.2f);
		fall.PushBack({ 250, 385, 51, 58 }, 0.2f);
		fall.PushBack({ 313, 385, 51, 58 }, 0.2f);
		fall.PushBack({ 374, 385, 51, 58 }, 0.2f);
		fall.PushBack({ 434, 385, 51, 58 }, 0.2f);
		fall.PushBack({ 494, 385, 51, 58 }, 0.2f);
		fall.PushBack({ 553, 385, 51, 58 }, 0.2f);
		fall.PushBack({ 614, 385, 51, 58 }, 0.2f);
		fall.loop = false;
		*/
	}

	LoadAnimations();

}

j1Player::~j1Player(){ }

bool j1Player::Awake(pugi::xml_node& config) {

	LOG("Loading Player Data");
	bool ret = true;

	current_animation = &idle;

	folder.create(config.child("folder").child_value());

	//set initial attributes of the player
	speed = config.child("speed").attribute("value").as_int();
	jumpImpulse = config.child("jumpImpulse").attribute("value").as_float();
	gravity = config.child("gravity").attribute("value").as_float();

	dieFX = config.child("dieFX").attribute("source").as_string();
	jumpFX = config.child("jumpFX").attribute("source").as_string();

	collider = App->collision->AddCollider(current_animation->GetCurrentFrame(), COLLIDER_PLAYER, (j1Module*)App->player); //a collider to start

	return ret;
}

bool j1Player::Start(){
	//load character sprites
	player_tex = App->tex->Load("sprites/characters/spritesheet_traveler.png");
	position.x = initial_x_position = App->scene->player_x_position;
	position.y = initial_x_position = App->scene->player_y_position;

	App->audio->LoadFx(dieFX.GetString());
	App->audio->LoadFx(jumpFX.GetString());

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
			velocity.y = 0;
		}
	}

	if (!App->pause)
	{
		velocity.x = 0;
		if (state == IDLE)
		{
			can_double_jump = true;

			if ((player_input.pressing_D)&&(velocity.y == 0))
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
			
			if (player_input.pressing_space)
			{
				state = JUMP;
				velocity.y = jumpImpulse;
				App->audio->PlayFx(1, 1);
				grounded = false;
			}
			
		}
		
		if (state == RUN_FORWARD)
		{
			if (!player_input.pressing_D)
			{
				state = IDLE;
				velocity.x = 0;
			}
			
			if (player_input.pressing_space)
			{
				state = JUMP;
				velocity.y = jumpImpulse;
				grounded = false;
			}
			
			if (player_input.pressing_F)
			{
				state = SLIDE_FORWARD;
			}

			velocity.x = speed;
		}

		if (state == RUN_BACKWARD)
		{
			if (!player_input.pressing_A)
			{
				state = IDLE;
				velocity.x = 0;
			}
			
			if (player_input.pressing_space)
			{
				state = JUMP;
				velocity.y = jumpImpulse;
				grounded = false;
			}
			
			if (player_input.pressing_F)
			{
				state = SLIDE_BACKWARD;
			}

			velocity.x = -speed;
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
			//position.x += speed;
			velocity.x = speed;
		}

		if (state == SLIDE_BACKWARD)
		{
			if (!player_input.pressing_F)
			{
				state = IDLE;
			}
			//position.x -= speed;
			velocity.x = -speed;
		}

		if (state == JUMP)
		{
			if (player_input.pressing_D) position.x += speed/2;
			if (player_input.pressing_A) position.x -= speed/2;

			if ((player_input.pressing_space)&&(can_double_jump == true)&&(velocity.y <= jumpImpulse/2))
			{ 
				jump.Reset();
				velocity.y = jumpImpulse * 2/3;
				can_double_jump = false;
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
			if ((player_input.pressing_D)&&(can_go_right == true)) position.x += speed /2;
			if ((player_input.pressing_A)&&(can_go_left == true)) position.x -= speed / 2;

			if ((player_input.pressing_space)&&(can_double_jump == true) & (velocity.y <= jumpImpulse / 2))
			{
				jump.Reset();
				state = JUMP;
				velocity.y = jumpImpulse * 2/3; 
				can_double_jump = false;
				grounded = false;
			}

			if (current_animation->Finished())
			{
				fall.Reset();
			}
		}

		if ((velocity.y < -10)&&(state == IDLE))
		{
			state = FALL;
		}

		MovementControl(); //calculate new position

		collider->SetPos(position.x, position.y);
	}

	if (god)
	{
		if (player_input.pressing_W) position.y -= speed;
		if (player_input.pressing_S) position.y += speed;
	}
	return true;
}

bool j1Player::Update(float dt){

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
		if (velocity.y <= 0)
		{
			state = FALL;
			jump.Reset();
		}
		if ((last_state = RUN_FORWARD)||(last_state == RUN_BACKWARD))
		{
			velocity.x /= 2;
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

	if (visible == true)
	{
		App->render->Blit(player_tex, position.x, position.y, &current_animation->GetCurrentFrame(), flip);
	}

	return true;	
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	if (!god)
	{
		switch (c2->type)
		{
			case COLLIDER_WALL:
				position = lastPosition;
				velocity.x = velocity.y = 0;
				if ((position.x < c2->rect.x + COLLIDER_MARGIN) && (state == FALL))
				{
					can_go_right = false;
				}
				if ((position.x > c2->rect.x + c2->rect.w - COLLIDER_MARGIN) && (state == FALL))
				{
					can_go_left = false;
				}
				if ((position.y < c2->rect.y + COLLIDER_MARGIN) && (last_state == FALL))
				{
					state = CROUCH_DOWN;
					fall.Reset();
					can_go_right = true;
					can_go_left = true;
				}

				break;
			case COLLIDER_DEATH:
				if (!god) {
					velocity.x = 0;
					if (App->ui->transition == false)
					{
						state = IDLE;
						//position.x = App->scene->player_x_position;
						//position.y = App->scene->player_y_position;
						App->ui->transition = true;
						App->scene->blocked_camera = true;
						App->ui->ResetTransition();
						//App->scene->Reset_Camera();
					}
				}
				break;
			case COLLIDER_PLATFORM:
				if (((position.y + current_animation->GetCurrentFrame().h >= c2->rect.y)&&(lastPosition.y + current_animation->GetCurrentFrame().h <= c2->rect.y + COLLIDER_MARGIN))||(grounded == true))
				{
					position = lastPosition;
					velocity.x = velocity.y = 0;
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
					//position.x = initial_x_position;
					//position.y = initial_y_position;
					App->ui->transition = true;
					App->ui->loaded = false;
					App->ui->transition_moment = SDL_GetTicks();
					App->ui->ResetTransition();
					//App->scene->Reset_Camera();
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

void j1Player::MovementControl() {
	position.x += velocity.x;
	position.y -= velocity.y;
	if (!god) velocity.y -= gravity;
	//LOG("velocity x. %.2f y: %.2f", velocity.x, velocity.y);
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