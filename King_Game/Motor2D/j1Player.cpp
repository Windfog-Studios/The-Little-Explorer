#include "j1Player.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"

j1Player::j1Player():j1Module () {
	name.create("player");

	//idle animation
	idle.PushBack({ 21, 226, 32, 56 });
	idle.PushBack({ 71, 226, 32, 56 });
	idle.PushBack({ 121, 226, 32, 56 });
	idle.PushBack({ 171, 226, 32, 56 });
	idle.PushBack({ 221, 226, 32, 56 });
	idle.PushBack({ 271, 226, 32, 56 });
	idle.PushBack({ 321, 226, 32, 56 });
	idle.PushBack({ 369, 226, 32, 56 });

	//walk animation
	walk.PushBack({ 21, 31, 33, 56 });
	walk.PushBack({ 70, 31, 32, 56 });
	walk.PushBack({ 119, 31, 30, 56 });
	walk.PushBack({ 169, 31, 30, 56 });
	walk.PushBack({ 215, 31, 30, 56 });
	walk.PushBack({ 264, 31, 30, 56 });
	walk.PushBack({ 312, 31, 30, 56 });
	walk.PushBack({ 359, 31, 30, 56 });

	//run animation
	run.PushBack({ 21, 156, 46, 53 });
	run.PushBack({ 79, 157, 44, 52 });
	run.PushBack({ 136, 155, 41, 54 });
	run.PushBack({ 196, 155, 46, 54 });
	run.PushBack({ 260, 157, 42, 52 });
	run.PushBack({ 322, 155, 40, 54 });

	//crouch animation
	crouch.PushBack({ 21, 302, 32, 56 });
	crouch.PushBack({ 75, 305, 35, 53 });
	crouch.PushBack({ 125, 321, 43, 37 });
	crouch.PushBack({ 184, 322, 43, 36 });

	//slide animation
	slide.PushBack({ 21, 101, 52, 42 });
	slide.PushBack({ 86, 101, 52, 42 });

	//jump animation
	jump.PushBack({ 420, 315, 49, 43 });
	jump.PushBack({ 477, 316, 52, 42 });
	jump.PushBack({ 539, 294, 32, 64 });
	jump.PushBack({ 582, 294, 32, 64 });
	jump.PushBack({ 626, 294, 32, 64 });
	jump.PushBack({ 669, 294, 32, 64 });
	jump.PushBack({ 710, 294, 32, 64 });
	jump.PushBack({ 752, 294, 32, 64 });
	jump.PushBack({ 796, 294, 32, 64 });
	jump.PushBack({ 37, 379, 32, 64 });
	jump.PushBack({ 78, 383, 37, 60 });
	jump.PushBack({ 124, 384, 47, 59 });
	jump.PushBack({ 183, 385, 51, 58 });
	jump.PushBack({ 250, 385, 51, 58 });
	jump.PushBack({ 313, 385, 51, 58 });
	jump.PushBack({ 374, 385, 51, 58 });
	jump.PushBack({ 434, 385, 51, 58 });
	jump.PushBack({ 494, 385, 51, 58 });
	jump.PushBack({ 553, 385, 51, 58 });
	jump.PushBack({ 614, 385, 51, 58 });


}

j1Player::~j1Player(){

}

bool j1Player::Awake(pugi::xml_node& config) {

	LOG("Loading Player Data");
	bool ret = true;
	position.x = 300;
	position.y = 100;
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
	player_input.pressing_A = App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN;
	player_input.pressing_S = App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN;
	player_input.pressing_D = App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN;

	return true;
}

bool j1Player::Update(){
	
	return true;
}

bool j1Player::PostUpdate() {

	App->render->Blit(player_tex, position.x, position.y, &idle.GetCurrentFrame());

	return true;
}