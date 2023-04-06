#include <stdio.h>
#include "ClickPicker.h"
#include <iostream>

void ClickPicker::Init(unsigned int width, unsigned int height) {
	
    // Create the FBO
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // Create the texture object for the primitive information buffer
    glGenTextures(1, &pixelTexture);
    glBindTexture(GL_TEXTURE_2D, pixelTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8UI, width, height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pixelTexture, 0);

    // Create the texture object for the depth buffer
    glGenTextures(1, &depthBuffer);
    glBindTexture(GL_TEXTURE_2D, depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

    // Verify that the FBO is correct
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        exit(1);
    }

    // Restore the default framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


ClickPicker::~ClickPicker()
{
	if (frameBuffer != 0) {
		glDeleteFramebuffers(1, &frameBuffer);
	}

	if (pixelTexture != 0) {
		glDeleteTextures(1, &pixelTexture);
	}

	if (depthBuffer != 0) {
		glDeleteTextures(1, &depthBuffer);
	}
}

void ClickPicker::Enable()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
}

void ClickPicker::Disable()
{
	// Bind back the default framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

ClickPicker::PixelData ClickPicker::Read(unsigned int x, unsigned int y)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);

    glReadBuffer(GL_COLOR_ATTACHMENT0);

    PixelData Pixel;

    glReadPixels(x, y, 1, 1, GL_RGB, GL_BYTE, &Pixel);
    //glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    return Pixel;
}