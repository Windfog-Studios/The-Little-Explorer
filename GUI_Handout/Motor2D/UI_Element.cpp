#include "j1UI_Element.h"
#include "j1App.h"
#include "j1Input.h"

bool j1UI_Element::MouseHovering() {
	iPoint mouse;
	bool ret = false;

	App->input->GetMousePosition(mouse.x, mouse.y);

	if ((mouse.x > rect.x) &&(mouse.x < rect.x + rect.w)
		&&(mouse.y > rect.y)&&(mouse.y < rect.y + rect.h))
	{
		ret = true;
	}

	return ret;
}