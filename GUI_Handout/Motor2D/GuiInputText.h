#ifndef _GUIINPUTTEXT_H_
#define _GUIINPUTTEXT_H
#include "GuiImage.h"
#include "GuiText.h"
#include "j1UI_Element.h"

class p2SString;

class GuiInputText : public j1UI_Element
{
public:
	GuiInputText(j1Module* callback);
	~GuiInputText();

	void Init(iPoint position, p2SString text, SDL_Rect image_section);
	bool Update(float dt);
	bool Input();
	bool Draw();

private:
	GuiImage* background;
	p2SString default_text;
	GuiText* text;
	SDL_Rect cursor;
	bool focused;
};


#endif // !_GUIINPUTTEXT_H_

