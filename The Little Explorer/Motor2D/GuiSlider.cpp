#include "GuiSlider.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Render.h"
#include "GuiText.h"
#include "p2SString.h"
#include "j1Fonts.h"

GuiSlider::GuiSlider(j1Module* g_callback) {
	callback = g_callback;
	text = new GuiText();
	click_rect = { 0,0,0,0 };
	texture = nullptr;
	current_rect = &normal_rect;
}

GuiSlider::~GuiSlider() {
	
}

void GuiSlider::Init() {
	
	scrollButton=(GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, callback, this, true, true);
	scrollButton->Init({ 285, 325 }, { 416,837,117,120 }, { 416,837,117,120 }, { 534,837,117,120 }, "");
	//scrollButton = App->gui->AddGuiElement(GUItype::GUI_BUTTON, this, globalPosition, localPosition, true, true, { 432, 36, 14 , 16 }, nullptr, this->listener, true, false);
	
	scrollButton->local_position.y = local_position.y - scrollButton->rect.h / 2 + this->rect.h / 2;
	int value = 0;

	texture = App->gui->GetAtlas();
	
}

bool GuiSlider::CleanUp() {
	return true;
}

bool GuiSlider::Input() {

	/*
	current_rect = &click_rect;
	callback->OnEvent(this, FocusEvent::CLICKED);
	*/
	return true;
}


bool GuiSlider::Update(float dt) {

	int ret = true;
	if (OnHover())
	{
		current_rect = &hover_rect;
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			current_rect = &click_rect;
		}
	}
	else
	{
		current_rect = &normal_rect;
	}

	if (parent != nullptr)
	{
		screen_position.x = parent->screen_position.x + local_position.x;
		screen_position.y = parent->screen_position.y + local_position.y;
	}

	rect.x = screen_position.x - App->render->camera.x;
	rect.y = screen_position.y - App->render->camera.y;

	if (text->text.Length() > 0)
	{
		text->Update(dt);
	}
	return ret;
}

bool GuiSlider::Draw() {
	
	App->render->Blit(texture, rect.x, rect.y, current_rect);
	if (text->text.Length() > 0) { text->Draw(); }
	
	return true;
}
