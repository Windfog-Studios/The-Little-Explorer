#include "GuiText.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"

GuiText::GuiText() {
	font = DEFAULT_FONT;
	text = { "No text" };
	callback = nullptr;
	parent = nullptr;
	texture = nullptr;
	draggable = false;
	interactable = false;
}

GuiText::GuiText(j1Module* g_callback) {
	callback = g_callback;
	font = DEFAULT_FONT;
	texture = nullptr;
	parent = nullptr;
	to_delete = false;
}

GuiText::~GuiText() {}

void GuiText::Init(iPoint g_position, p2SString g_text, char* g_font) {
	screen_position = g_position;
	text = g_text;
	font = g_font;

	if (text.Length() > 0)
	{
		if (font == DEFAULT_FONT)
		{
			texture = App->font->Print(text.GetString());
			App->font->CalcSize(text.GetString(), rect.w, rect.h);
		}
		else
		{
			texture = App->font->Print(text.GetString(), { (255),(255),(255),(255) }, App->font->console_font);
			App->font->CalcSize(text.GetString(), rect.w, rect.h, App->font->console_font);
		}
	}

	if (parent != nullptr)
	{
		local_position.x = screen_position.x -parent->screen_position.x;
		local_position.y = screen_position.y -parent->screen_position.y;
	}

	rect.x = screen_position.x;
	rect.y = screen_position.y;
}

bool GuiText::CleanUp() {
	bool ret = true;
	App->tex->UnLoad(texture);
	texture = nullptr;
	parent = nullptr;
	return ret;
}

bool GuiText::Update(float dt) {

	bool ret = true;

	if (parent != nullptr)
	{
		screen_position.x = parent->screen_position.x + local_position.x;
		screen_position.y = parent->screen_position.y + local_position.y;
	}
	
	rect.x = screen_position.x -App->render->camera.x;
	rect.y = screen_position.y -App->render->camera.y;
	
	return ret;
}

bool GuiText::Draw() {

	if (text.Length() > 0)
	{
		if (texture != nullptr)
		{
			App->render->Blit(texture, rect.x, rect.y);
		}
	}

	return true;
}

void GuiText::UpdateText() {

	if (text.Length() > 0)
	{
		if (font == DEFAULT_FONT)
		{
			App->tex->UnLoad(texture);
			texture = nullptr;
			texture = App->font->Print(text.GetString());
		}
		else
		{
			App->tex->UnLoad(texture);
			texture = nullptr;
			texture = App->font->Print(text.GetString(), { (255),(255),(255),(255) }, App->font->console_font);
		}
	}
}