#include "j1Console.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1Command.h"

j1Console::j1Console() : j1Module() {
	isVisible = false;
}

j1Console::~j1Console() {

}

bool j1Console::Awake(pugi::xml_node& config) {
	bool ret = true;

	return ret;
}

bool j1Console::Start() {
	bool ret = true;
	GuiText* log_text;
	log_text = (GuiText*)App->gui->CreateUIElement(UI_Type::TEXT, this, nullptr);
	log_text->Init({ 20,0 }, " ");
	log_record.add(log_text);
	return ret;
}

bool j1Console::PreUpdate() {
	bool ret = true;
	return ret;
}

bool j1Console::Update(float dt) {
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN)
	{
		if (isVisible)
		{
			DestroyInterface();
		}
		else
		{
			CreateInterface();
		}
	}

	if (isVisible) {
		if ((App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)) 
		{
			DestroyInterface();
		}
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			p2SString input_text(App->input->text);
			App->console->AddLogText(input_text);
			command_input->GetText()->text.Clear();

			j1Command* command = new j1Command(input_text, this);
			commands.add(command);

			CheckCommand(command);

		}

		command_input->Input();
	}
	return ret;
}

bool j1Console::PostUpdate() {
	bool ret = true;

	if (isVisible)
	{
		log_box = {(int) -App->render->camera.x,(int) -App->render->camera.y, (int) App->win->width, 350 };
		command_background = { -App->render->camera.x,(int)(log_box.h - App->render->camera.y), (int) App->win->width, 40 };
		command_input->rect = command_background;

		//Draw console
		App->render->DrawQuad(log_box, 0, 0, 0, 255);
		App->render->DrawQuad(command_background, 0, 0, 255, 255);
		command_input->Draw();

		for (p2List_item<GuiText*>* item = log_record.start; item != nullptr; item = item->next)
		{
			item->data->Draw();
		}

		//App->gui->DebugDraw();
	}
	return ret;
}

bool j1Console::CleanUp() {
	bool ret = true;
	return ret;
}

void j1Console::CreateInterface(){
	command_input = (GuiInputText*)App->gui->CreateUIElement(UI_Type::INPUT_TEXT, this, nullptr, false, true);
	command_input->Init({ 0, (int)(log_box.h - App->render->camera.y) }, "Write Command", { 0,(int)(log_box.y + log_box.h),(int)App->win->width, command_background.h});

	App->gui->focused_element = command_input;
	command_input->HandleFocusEvent(FocusEvent::FOCUS_GAINED);
	
	isVisible = true;
}

void j1Console::DestroyInterface(){
	App->gui->DestroyUIElement(command_input);
	for (p2List_item<GuiText*>* item = log_record.start; item != nullptr; item = item->next)
	{
		App->gui->DestroyUIElement(item->data);
	}
	isVisible = false;
}

void j1Console::AddLogText(p2SString new_text) {
	GuiText* log_text;
	log_text = (GuiText*)App->gui->CreateUIElement(UI_Type::TEXT, this, nullptr);
	if(log_record.end == nullptr) 	log_text->Init({ 20,20 }, new_text);
	else log_text->Init({ 20,(int)(log_record.end->data->rect.y + log_record.end->data->rect.h) }, new_text);
	log_record.add(log_text);
	if ((log_record.start->data->rect.h * log_record.count()) > log_box.h)
	{
		log_record.start->data->rect.y -= log_text->rect.h;
	}
}

void j1Console::CheckCommand(j1Command* command) {
	/*
	int l = command->text.Length();
	char* initial_command = (char*)command->text.GetString();
	char* lowercased_command = new char[l];

	for (int i = 0; i < l; i++)
	{
		lowercased_command[i] = tolower(initial_command[i]);
	}
	
	p2SString final_command(lowercased_command);
	
	if (final_command == "quit")
	{
		App->quit = true;
	}
	*/
}