#include "j1Console.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"

j1Console::j1Console() : j1Module() {
	isVisible = false;
}

j1Console::~j1Console() {

}

bool j1Console::Awake(pugi::xml_node& config) {
	bool ret = true;
	return ret;
}

bool j1Console::PreUpdate() {
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_GRAVE))
	{
		if (isVisible)
		{
			DestroyInterface();
			isVisible = false;
		}
		else
		{
			CreateInterface();
			isVisible = true;
		}
	}
	return ret;
}

bool j1Console::PostUpdate() {
	bool ret = true;

	if (isVisible)
	{
		log_box = {(int) -App->render->camera.x,(int) -App->render->camera.y, (int) App->win->width, 200 };
		command_background = { -App->render->camera.x,(int)(log_box.h - App->render->camera.y), (int) App->win->width, 30 };

		App->render->DrawQuad(log_box, 40, 40, 40, 255);
		App->render->DrawQuad(command_background, 0, 0, 255, 255);
		command_input->Draw();
	}
	return ret;
}

bool j1Console::CleanUp() {
	bool ret = true;
	return ret;
}

void j1Console::CreateInterface(){
	command_input = (GuiInputText*)App->gui->CreateUIElement(UI_Type::INPUT_TEXT, this, nullptr, false, true);
	command_input->Init({ 0,200 }, "Write Command", { 0,(int)(log_box.y + log_box.h),(int)App->win->width, command_background.h});
}

void j1Console::DestroyInterface(){
	App->gui->DestroyUIElement(command_input);
}