#ifndef _GUITEXT_H_
#define _GUITEXT_H_
#include "j1UI_Element.h"
#include "j1Fonts.h"

class GuiText : public j1UI_Element
{
public:
	GuiText();
	GuiText(j1Module* callback);
	~GuiText();

	void Init(iPoint position, p2SString text);
	bool Update(float dt);
	bool Draw();

private:
	p2SString		text;
	const char*		font;
	SDL_Texture*	texture = nullptr;
};

#endif // !_GUISTATICTEXT_H_
