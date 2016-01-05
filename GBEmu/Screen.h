#pragma once

#include <SDL.h>
#include <SDL_render.h>
#include <vector>
const unsigned short ScreenWidth = 64;
const unsigned short ScreenHeight = 32;

class Screen
{
public:
	Screen();
	~Screen();

	bool TogglePixel(unsigned short x, unsigned short y);
	void Clear();
	void DrawFrame();
private:

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	std::vector< unsigned char > pixels;
};

