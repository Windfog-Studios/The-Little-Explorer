#include "j1Player.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"

j1Player::j1Player():j1Module () {
	name.create("player");

	//idle animation
	idle.PushBack({ 21, 226, 32, 56 }, 0.1f);
	idle.PushBack({ 71, 226, 32, 56 }, 0.1f);
	idle.PushBack({ 121, 226, 32, 56 }, 0.1f);
	idle.PushBack({ 171, 226, 32, 56 }, 0.1f);
	idle.PushBack({ 221, 226, 32, 56 }, 0.1f);
	idle.PushBack({ 271, 226, 32, 56 }, 0.1f);
	idle.PushBack({ 321, 226, 32, 56 }, 0.1f);
	idle.PushBack({ 369, 226, 32, 56 }, 0.1f);

	//walk animation
	walk.PushBack({ 21, 31, 33, 56 }, 0.2f);
	walk.PushBack({ 70, 31, 32, 56 }, 0.2f);
	walk.PushBack({ 119, 31, 30, 56 }, 0.2f);
	walk.PushBack({ 169, 31, 30, 56 }, 0.2f);
	walk.PushBack({ 215, 31, 30, 56 }, 0.2f);
	walk.PushBack({ 264, 31, 30, 56 }, 0.2f);
	walk.PushBack({ 312, 31, 30, 56 }, 0.2f);
	walk.PushBack({ 359, 31, 30, 56 }, 0.2f);

	//run animation
	run.PushBack({ 21, 156, 46, 53 }, 0.2f);
	run.PushBack({ 79, 157, 44, 52 }, 0.2f);
	run.PushBack({ 136, 155, 41, 54 }, 0.2f);
	run.PushBack({ 196, 155, 46, 54 }, 0.2f);
	run.PushBack({ 260, 157, 42, 52 }, 0.2f);
	run.PushBack({ 322, 155, 40, 54 }, 0.2f);

	//crouch animation
	crouch.PushBack({ 21, 302, 32, 56 }, 0.02f);
	crouch.PushBack({ 75, 305, 35, 53 }, 0.02f);
	crouch.PushBack({ 125, 321, 43, 37 }, 0.02f);
	crouch.PushBack({ 184, 322, 43, 36 }, 0.02f);

	//slide animation
	slide.PushBack({ 21, 101, 52, 42 }, 0.2f);
	slide.PushBack({ 86, 101, 52, 42 }, 0.2f);

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
	jump.PushBack({ 37, 379, 32, 64 }, 0.2f);
	jump.PushBack({ 78, 383, 37, 60 }, 0.2f);
	jump.PushBack({ 124, 384, 47, 59 }, 0.2f);
	jump.PushBack({ 183, 385, 51, 58 }, 0.2f);
	jump.PushBack({ 250, 385, 51, 58 }, 0.2f);
	jump.PushBack({ 313, 385, 51, 58 }, 0.2f);
	jump.PushBack({ 374, 385, 51, 58 }, 0.2f);
	jump.PushBack({ 434, 385, 51, 58 }, 0.2f);
	jump.PushBack({ 494, 385, 51, 58 }, 0.2f);
	jump.PushBack({ 553, 385, 51, 58 }, 0.2f);
	jump.PushBack({ 614, 385, 51, 58 }, 0.2f);


}

j1Player::~j1Player(){

}

bool j1Player::Awake(pugi::xml_node& config) {

	LOG("Loading Player Data");
	bool ret = true;
	position.x = 50;
	position.y = 450;
	current_animation = &idle;
	//set initial position
	//position.x = config.child("position").attribute("x").as_int();
	//position.y = config.child("position").attribute("y").as_int();

	return ret;
}

bool j1Player::Start(){

	player_tex = App->tex->Load("sprites/characters/Spritesheet_traveler.png");	//load character sprites

	return true;
}

bool j1Player::CleanUp() {
	return true;
}

bool j1Player::PreUpdate(){
	//get player input
	player_input.pressing_W = App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN;
	player_input.pressing_A = App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	player_input.pressing_S = App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	player_input.pressing_D = App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
	player_input.pressing_F = App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT;
	
	if (state == IDLE)
	{
		if (player_input.pressing_D)
		{
			position.x++;
			state = RUN_FORWARD;
		}

		if (player_input.pressing_A)
		{
			position.x--;
			state = RUN_BACKWARD;
		}
		//new states
		/*
		if (player_input.pressing_S)
		{
			state = CROUCH;
			current_animation = &crouch;
		}

		if (player_input.pressing_F)
		{
			position.x++;
			state = SLIDE;
			current_animation = &slide;
		}
		*/
	}

	if (state == RUN_FORWARD)
	{
		if (!player_input.pressing_D)
		{
			state = IDLE;
		}
		position.x++;

	}

	if (state == RUN_BACKWARD)
	{
		if (!player_input.pressing_A)
		{
			state = IDLE;
		}
		position.x--;
	}

	/*
	if (state == CROUCH)
	{
		if (!player_input.pressing_S)
		{
			state = IDLE;
			current_animation = &idle;
		}
	}
	
	if (state == SLIDE)
	{
		if (!player_input.pressing_F)
		{
			state = IDLE;
			current_animation = &idle;
		}
		position.x++;
	}
	*/

	return true;
}

bool j1Player::Update(float dt){

	switch (state)
	{
	case IDLE:
		current_animation = &idle;
		//flip = SDL_FLIP_NONE;
		break;

	case RUN_FORWARD:
		current_animation = &run;
		flip = SDL_FLIP_NONE;
		break;

	case RUN_BACKWARD:
		current_animation = &run;
		flip = SDL_FLIP_HORIZONTAL;
		break;
/*
	case CROUCH:
		current_animation = &crouch;
		break;

	case SLIDE:
		current_animation = &slide;
		break;
*/
	default:
		LOG("No state found");
		break;
	}

	return true;
}

bool j1Player::PostUpdate() {

	App->render->Blit(player_tex, position.x, position.y, &current_animation->GetCurrentFrame(), flip);

	return true;	
}