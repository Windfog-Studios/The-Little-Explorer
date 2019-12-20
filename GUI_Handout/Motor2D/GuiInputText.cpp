#include "GuiInputText.h"
#include "j1Render.h"
#include "j1App.h"


GuiInputText::GuiInputText(j1Module* g_callback){
	background = new GuiImage(g_callback);
	text = new GuiText(g_callback);
	callback = g_callback;
}

GuiInputText::~GuiInputText(){
	delete background;
	delete text;
}

void GuiInputText::Init(iPoint position, p2SString g_text, SDL_Rect image_section){
	
	screen_position = position;
	rect = { position.x, position.y, image_section.w, image_section.h };

	background->parent = text->parent = (j1UI_Element*)this;
	background->draggable = text->draggable = false;

	rect.x = screen_position.x;
	rect.y = screen_position.y;

	if (parent != nullptr)
	{
		local_position.x = screen_position.x - parent->screen_position.x;
		local_position.y = screen_position.y - parent->screen_position.y;
	}

	background->Init(position, image_section);
	text->Init({position.x + 10,(int)(position.y + rect.h * 0.25f)}, g_text);
}

bool GuiInputText::Input() {
	return true;
}

bool GuiInputText::Update(float dt) {
	bool ret = true;

	if (parent != nullptr)
	{
		screen_position.x = parent->screen_position.x + local_position.x;
		screen_position.y = parent->screen_position.y + local_position.y;
	}

	rect.x = screen_position.x;
	rect.y = screen_position.y;

	background->Update(dt);
	text->Update(dt);

	return ret;
}

bool GuiInputText::Draw() {

	background->Draw();
	text->Draw();

	return true;
}