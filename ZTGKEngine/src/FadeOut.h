#pragma once

#include "UI/ApRectangle.h"
#include "GraphNode.h"

#define PARTICLE_AMOUNT 8

class FadeOut {

private:
	unsigned int positions[8][2]; // 0 - width, 1 - height
	Shader* particleShader;

public:
	ApRectangle* particles[PARTICLE_AMOUNT];
	FadeOut(string texture_path, unsigned int* scr_width, unsigned int* scr_height, Shader* shader)
	{
		unsigned int w_part = *scr_width / 16;
		unsigned int h_part = *scr_height / 9;

		positions[0][0] = 0;
		positions[0][1] = 0;

		positions[1][0] = 0;
		positions[1][1] = h_part * 6;

		positions[2][0] = w_part * 2;
		positions[2][1] = h_part * 3;

		positions[3][0] = w_part * 6;
		positions[3][1] = h_part;

		positions[4][0] = w_part * 6;
		positions[4][1] = h_part * 4;

		positions[5][0] = w_part * 10;
		positions[5][1] = h_part * 3;

		positions[6][0] = w_part * 12;
		positions[6][1] = 0;

		positions[7][0] = w_part * 12;
		positions[7][1] = h_part * 6;

		for (int i = 0; i < PARTICLE_AMOUNT; i++) {
			particles[i] = new ApRectangle(positions[i][0], positions[i][1], w_part * 4, h_part * 3, texture_path);
			particles[i]->SetShader(shader);
		}
	}

	void Draw() {
		for (int i = 0; i < PARTICLE_AMOUNT; i++) {
			particles[i]->Draw();
		}
	}

};