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
		float ObjectID = 0.0f;
		float DrawID = 0.0f;
		float PrimID = 0.0f;
	};

	PixelData Read(int x, int y);

	unsigned int activeID;
private:
	GLuint frameBuffer = 0;
	GLuint pixelTexture = 0;
	GLuint depthBuffer = 0;
};

#endif