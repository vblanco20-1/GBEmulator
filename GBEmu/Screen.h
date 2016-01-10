#pragma once

#include <SDL.h>
#include <SDL_render.h>
#include <vector>
const unsigned short ScreenWidth = 256;
const unsigned short ScreenHeight = 256;

class Screen
{
public:
	Screen();
	~Screen();

	bool TogglePixel(unsigned short x, unsigned short y);
	void SetPixel(unsigned short x, unsigned short y, int color);
	void Clear();
	void DrawFrame();
	void DrawTile(std::vector<uint8_t> &tile, uint32_t x, uint32_t y);
	void TestDraw(unsigned char * first, unsigned int sprites);
	
	void SetScrollX(uint8_t scrollx) {};
	void SetScrollY(uint8_t scrolly) {};

	void DrawBackground();

	uint8_t vram[0xF000];
private:
	


	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	std::vector< unsigned char > pixels;
};

