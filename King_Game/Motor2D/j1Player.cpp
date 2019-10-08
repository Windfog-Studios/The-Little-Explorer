#include "j1Player.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"

j1Player::j1Player():j1Module () {
	name.create("player");
}

j1Player::~j1Player(){

}

bool j1Player::Awake(pugi::xml_node& config) {
	LOG("Loading Player Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

bool j1Player::Start(){
	player_tex = App->tex->Load("Game/sprites/characters/characters.png");
	return true;
}

bool j1Player::CleanUp() {
	return true;
}

bool j1Player::PreUpdate(){

	player_input.pressing_W = App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN;
	player_input.pressing_A = App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN;
	player_input.pressing_S = App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN;
	player_input.pressing_D = App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN;

	return true;
}

bool j1Player::Update(){
	
	return true;
}

bool j1Player::PostUpdate() {

	return true;
}