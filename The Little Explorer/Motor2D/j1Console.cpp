#include "j1Console.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1Command.h"
#include "p2Log.h"

j1Console::j1Console() : j1Module() {
	isVisible = false;
	CleanUpStarted = false;
	l = 0;
	command_input = nullptr;
	current_consulting_command = nullptr;
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

	CreateCommand("help", (j1Module*)this, 0, 0, "List all console commands");

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

			input_commands.add(input_text);

			CheckCommand(input_text);
		}

		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
			if (input_commands.end != nullptr)
			{
				if (current_consulting_command == nullptr)
				{
					current_consulting_command = input_commands.end;
					command_input->GetText()->text = current_consulting_command->data;
				}
				else
				{
					if (current_consulting_command->prev != nullptr) {
						current_consulting_command = current_consulting_command->prev;
						command_input->GetText()->text = current_consulting_command->data;
					}
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
			if (input_commands.end != nullptr)
			{
				if (current_consulting_command == nullptr)
				{
					current_consulting_command = input_commands.end;
					command_input->GetText()->text = current_consulting_command->data;
				}
				else
				{
					if (current_consulting_command->next != nullptr) {
						current_consulting_command = current_consulting_command->next;
						command_input->GetText()->text = current_consulting_command->data;
					}
				}
			}
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

		App->gui->DebugDraw();
	}
	return ret;
}

bool j1Console::CleanUp() {
	bool ret = true;
	CleanUpStarted = true;
	return ret;
}

void j1Console::CreateInterface(){
	command_input = (GuiInputText*)App->gui->CreateUIElement(UI_Type::INPUT_TEXT, this, nullptr, false, true);
	command_input->Init({ 0, (int)(log_box.h - App->render->camera.y) }, "Write Command", { 0,(int)(log_box.y + log_box.h),(int)App->win->width, command_background.h});

	App->gui->focused_element = command_input;
	command_input->HandleFocusEvent(FocusEvent::FOCUS_GAINED);
	
	for (p2List_item<GuiText*>* item = log_record.start; item != nullptr; item = item->next)
	{
		App->gui->ui_elements.add(item->data);
	}

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
	if (!CleanUpStarted)
	{
		GuiText* log_text;

		log_text = (GuiText*)App->gui->CreateUIElement(UI_Type::TEXT, this, nullptr);

		if (log_record.end == nullptr) 
			log_text->Init({ 20,20 }, new_text);

		else {
			log_text->parent = log_record.end->data;
			log_text->Init({ 20,(int)(log_record.end->data->rect.y + log_record.end->data->rect.h) }, new_text);
		}

		log_record.add(log_text);

		if ((log_record.end->data->rect.y + log_record.end->data->rect.h) > log_box.h)
		{
			log_record.start->data->screen_position.y -= log_record.end->data->rect.h;
		}
	}
}

void j1Console::CheckCommand(p2SString command) {

	p2SString final_command;
	final_command = command.lowercased();
	/*
	if (final_command == "quit")
	{
		App->quit = true;
	}
	*/
	
	p2SString comparing_command;
	for (p2List_item<j1Command*>* item = commands.start; item != nullptr; item = item->next)
	{
		comparing_command = item->data->text;
		comparing_command.lowercased();
		
		if (final_command == comparing_command)
		{
			item->data->callback->OnCommand(item->data);
		}
	}
}

void j1Console::CreateCommand(const char* g_command, j1Module* g_callback, uint min_args, uint max_args, const char* explanation) {
	j1Command* command = new j1Command(g_command, g_callback,min_args,max_args,explanation);
	commands.add(command);
}

void j1Console::OnCommand(j1Command* command) {
	char* command_text = (char*)command->text.GetString();

	if ( strcmp(command_text, "help") == 0)
	{
		for (p2List_item<j1Command*>* item = commands.start; item != nullptr; item = item->next)
		{
			//LOG(("%s : %s \n", item->data->text.GetString(), item->data->explanation);
			char string[200];
			strcpy_s(string, item->data->text.GetString());
			strcat_s(string, " : ");
			strcat_s(string, item->data->explanation);
			AddLogText(string);
		}
	}
}