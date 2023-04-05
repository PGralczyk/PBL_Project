#ifndef CLICKPICKER_H
#define CLICKPICKER_H

#include <glad/glad.h>

class ClickPicker {

public:
	ClickPicker() {}

	~ClickPicker();

	void Init(unsigned int width, unsigned int height);

	void Enable();

	void Disable();

	struct PixelData {
		unsigned int ObjectID = 0;
		unsigned int DrawID = 0;
		unsigned int PrimID = 0;
	};

	PixelData Read(unsigned int x, unsigned int y);

private:
	GLuint frameBuffer = 0;
	GLuint pixelTexture = 0;
	GLuint depthBuffer = 0;
};

#endif