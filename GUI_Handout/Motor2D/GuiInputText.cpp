#include "GuiInputText.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Input.h"


GuiInputText::GuiInputText(j1Module* g_callback){
	background = new GuiImage(g_callback);
	text = new GuiText(g_callback);
	callback = g_callback;
	default_text = { "Input text" };
	cursor = {0,0,2,30};
	focused = false;
}

GuiInputText::~GuiInputText(){
	delete background;
	delete text;
}

void GuiInputText::Init(iPoint position, p2SString g_text, SDL_Rect image_section){
	
	screen_position = position;
	rect = { position.x, position.y, image_section.w, image_section.h };
	default_text = g_text;

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
	focused = true;
	return true;
}

bool GuiInputText::Update(float dt) {
	bool ret = true;
	int width;
	
	if (parent != nullptr)
	{
		screen_position.x = parent->screen_position.x + local_position.x;
		screen_position.y = parent->screen_position.y + local_position.y;
	}

	rect.x = screen_position.x;
	rect.y = screen_position.y;

	background->Update(dt);
	text->Update(dt);

	if (text->text.GetCapacity() > 0)
	{
		App->font->CalcSize(text->text.GetString(), width, cursor.y);
		cursor.x = background->GetScreenRect().x + width;
		cursor.y = background->GetScreenRect().y + background->GetLocalRect().h * 0.5f - cursor.h * 0.5f;
	}


	return ret;
}

bool GuiInputText::Draw() {

	background->Draw();
	text->Draw();
	if (focused)
	{
		App->render->DrawQuad(cursor, 255, 255, 255, 255);
	}
	return true;
}

void GuiInputText::HandleFocusEvent(FocusEvent event) {
	if (event == FocusEvent::FOCUS_GAINED) {
		if (text->text == default_text)
		{
			text->text.Clear();
		}
		App->input->EnableTextInput(true);
		App->input->text = (char* )text->text.GetString();
	}
	
	if (event == FocusEvent::FOCUS_LOST)
	{
		focused = false;
		App->input->EnableTextInput(false);
	}
}