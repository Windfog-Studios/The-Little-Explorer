#include "j1Player.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"

j1Player::j1Player():j1Module () {
	name.create("player");
	idle.PushBack({8,41,16,23});
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

	player_tex = App->tex->Load("sprites/characters/characters.png");	//load character sprites

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