#include "ClickPicker.h"

ClickPicker texturePicker;

void PickPhase() {

	texturePicker.Enable();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//clickpick shader.use()

	//for each mesh drawn: (world->children amount?)
	//mvp -> clickpick shader
	//objectIndex -> clickpick shader
	//pass mesh ID to DrawIndex in clickpick shader (in Mesh Render()?)
	//world->draw()
	////add clickable functionality?
	texturePicker.Disable();

}

void ClickEvent() {

	//Pixel = ClickPicker.ReadPixel(m_leftMouseButton.x, WINDOW_HEIGHT - m_leftMouseButton.y - 1)
	//objectID = Pixel.ObjectID - 1;
	//for each mesh(model?) drawn:
	//if i == objectID:
	//color me (surprised) or printf model name or just ID xd
	//finally render normally

}