#ifndef _GUISLIDER_H_
#define _GUISLIDER_H_
#include "p2Point.h"
#include "j1UI_Element.h"
#include "SDL_image/include/SDL_image.h"

class GuiImage;

class GuiSlider : public j1UI_Element
{
public:

	GuiSlider(j1Module* callback);
	~GuiSlider();

	void Init(iPoint position, SDL_Rect scrollbar_section, SDL_Rect scroll_thumb_section);
	bool Update(float dt);
	bool CleanUp();
	bool Input();
	bool Draw();

private:
	GuiImage*		scrollbar;
	GuiImage*		scroll_thumb;
	float			output_value;
	//SDL_Rect*		current_rect;
	//SDL_Rect		normal_rect;
	//SDL_Rect		hover_rect;
	//SDL_Rect		click_rect;
public:
	//ButtonAction	action;
};


#endif // !_GUISLIDER_H_
