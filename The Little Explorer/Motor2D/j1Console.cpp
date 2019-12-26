#include "j1Console.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1Command.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "brofiler/Brofiler/Brofiler.h"

j1Console::j1Console() : j1Module() {
	isVisible = false;
	CleanUpStarted = false;
	l = 0;
	command_input = nullptr;
	current_consulting_command = nullptr;
}

j1Console::~j1Console() {}

bool j1Console::Awake(pugi::xml_node& config) {
	bool ret = true;

	return ret;
}

bool j1Console::Start() {
	bool ret = true;
	//�GuiText* log_text;
	//log_text = (GuiText*)App->gui->CreateUIElement(UI_Type::TEXT, this, nullptr, false, false, true);
	//log_text = (GuiText*)App->gui->CreateUIElement(UI_Type::TEXT, this);
	//log_text->Init({ 20,20 }, "Console Started", CONSOLE_FONT);
	AddLogText("Console started");

	CreateCommand("list", (j1Module*)this, "List all console commands");
	App->console->CreateCommand("quit", (j1Module*)this, "Quit the game");
	App->console->CreateCommand("fps_", (j1Module*)this, "Change FPS cap");

	return ret;
}

bool j1Console::PreUpdate() {
	bool ret = true;
	return ret;
}

bool j1Console::Update(float dt) {
	BROFILER_CATEGORY("ConsoleUpdate", Profiler::Color::MediumPurple)
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
	BROFILER_CATEGORY("GuiPostUpdate", Profiler::Color::DarkViolet)
	bool ret = true;

	if (isVisible)
	{
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
	CleanUpStarted = true;

	for (p2List_item<GuiText*>* item = log_record.start; item != nullptr; item = item->next)
	{
		App->gui->DestroyUIElement(item->data);
		log_record.del(item);
	}

	return ret;
}

void j1Console::CreateInterface(){
	log_box = { 0, 0, (int)App->win->width, 350 };
	command_background = { 0, log_box.h, (int)App->win->width, 40 };

	command_input = (GuiInputText*)App->gui->CreateUIElement(UI_Type::INPUT_TEXT, this, nullptr, false, true);
	command_input->Init({ 0, log_box.h }, "Write Command", { 0,(int)(log_box.y + log_box.h),(int)App->win->width, command_background.h}, false,CONSOLE_FONT);
	command_input->rect = command_background;

	App->gui->focused_element = command_input;
	command_input->HandleFocusEvent(FocusEvent::FOCUS_GAINED);

	isVisible = true;
	App->entities->blocked_movement = true;

	log_record.start->data->rect.x = 20;
	log_record.start->data->rect.y = 10;

	if (log_record.start->next != nullptr)
	{
		for (p2List_item<GuiText*>* item = log_record.start->next; item != nullptr; item = item->next)
		{
			item->data->rect.x = log_record.start->data->rect.x;
			item->data->rect.y = item->prev->data->rect.y + item->prev->data->rect.h;
		}
	}

	if (log_record.end->data->rect.y + log_record.end->data->rect.h > log_box.y + log_box.h)
	{
		log_record.start->data->rect.y -= log_record.end->data->rect.y - command_input->rect.y + log_record.end->data->rect.h;
		for (p2List_item<GuiText*>* item = log_record.start->next; item != nullptr; item = item->next)
		{
			item->data->rect.x = log_record.start->data->rect.x;
			item->data->rect.y = item->prev->data->rect.y + item->prev->data->rect.h;
		}
	}
}

void j1Console::DestroyInterface(){
	App->gui->DestroyUIElement(command_input);

	for (p2List_item<GuiText*>* item = log_record.start; item != nullptr; item = item->next)
	{
		App->gui->DestroyUIElement(item->data);
	}

	isVisible = false;
	App->entities->blocked_movement = false;
}

void j1Console::AddLogText(p2SString new_text) {
	if (!CleanUpStarted)
	{
		GuiText* log_text;

		log_text = (GuiText*)App->gui->CreateUIElement(UI_Type::TEXT,this);

		if (log_record.end == nullptr)
		{
			log_text->parent = command_input;
			log_text->Init({ 20, 20 }, new_text, CONSOLE_FONT);
		}
		else {
			log_text->parent = log_record.start->data;
			log_text->Init({ 20,(int)(log_record.end->data->rect.y + log_record.end->data->rect.h) }, new_text, CONSOLE_FONT);

			if ((command_input != nullptr)&&(log_record.end->data->rect.y + log_record.end->data->rect.h > log_box.y + log_box.h))
			{
				log_record.start->data->rect.y -= log_record.end->data->rect.h;
				for (p2List_item<GuiText*>* item = log_record.start->next; item != nullptr; item = item->next)
				{
					item->data->rect.x = log_record.start->data->rect.x;
					item->data->rect.y = item->prev->data->rect.y + item->prev->data->rect.h;
				}
			}
		}

		log_record.add(log_text);
	}
}

void j1Console::CreateCommand(const char* g_command, j1Module* g_callback, const char* explanation) {
	j1Command* command = new j1Command(g_command, g_callback,explanation);
	commands.add(command);
}

void j1Console::CheckCommand(p2SString command) {
	char given_initial_three[5] = "0000";
	char listed_initial_three[5] = "0000";
	char* given_command = (char* )command.GetString();
	

	for (p2List_item<j1Command*>* item = commands.start; item != nullptr; item = item->next)
	{
		if (item->data->text == command)
		{
			item->data->callback->OnCommand(command);
			return;
		}

		for (int i = 0; i < 4; i++)
		{
			given_initial_three[i] = command.GetString()[i];
			listed_initial_three[i] = item->data->text.GetString()[i];
		}

		if (strcmp(given_initial_three, listed_initial_three) == 0)
		{
			item->data->callback->OnCommand(command);
			return;
		}
	}

	LOG("Invalid command");
}

void j1Console::OnCommand(p2SString command) {
	char fps_string[8] = "fps_120";
	char initial_three[5] = "0000";

	if ( command == "list")
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
		return;
	}

	if (command == "quit") {
		App->quit = true;
		return;
	}

	for (int i = 0; i < 3; i++)
	{
		initial_three[i] = command.lowercased().GetString()[i];
	}
	
	if (strcmp(initial_three, "fps") == 0);
	{
		int fps = 0;
		if (command.Length() == 7)
		{
			char fps_char[3];
			fps_char[0] = command.GetString()[4];
			fps_char[1] = command.GetString()[5];
			fps_char[2] = command.GetString()[6];

			int units = (int)fps_char[2] - 48;
			int dozens = (int)fps_char[1] - 48;
			int hundreds = (int)fps_char[0] - 48;
			fps = units + dozens * 10 + hundreds * 100;
		}
		else if (command.Length() == 6)
		{
			char fps_char[2];
			fps_char[0] = command.GetString()[4];
			fps_char[1] = command.GetString()[5];
			int units = (int)fps_char[1] - 48;
			int dozens = (int)fps_char[0] - 48;
			fps = units + dozens * 10;
		}
		else
		{
			LOG("Invalid framerate cap number");
			return;
		}

		if (fps < 30) {
			LOG("Too low framerate cap");
		}
		else if(fps > 120)
		{
			LOG("Too high framerate cap");
		}
		else
		{
			App->framerate_cap = fps;
			LOG("Framerate cap changed to %i", fps);
		}
	}
}