#include "Screen.h"
#include <iostream>
using namespace std;


Screen::Screen()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = window = SDL_CreateWindow
		(
			"SDL2",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			1200, 600,
			SDL_WINDOW_SHOWN
			);

	renderer = SDL_CreateRenderer
		(
			window,
			-1,
			SDL_RENDERER_ACCELERATED
			);

	SDL_RendererInfo info;
	SDL_GetRendererInfo(renderer, &info);
	cout << "Renderer name: " << info.name << endl;
	cout << "Texture formats: " << endl;
	for (Uint32 i = 0; i < info.num_texture_formats; i++)
	{
		cout << SDL_GetPixelFormatName(info.texture_formats[i]) << endl;
	}

	const unsigned int texWidth = ScreenWidth;
	const unsigned int texHeight = ScreenHeight;
	texture = SDL_CreateTexture
		(
			renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			texWidth, texHeight
			);

	vector< unsigned char > px(texWidth * texHeight * 4, 0);
	pixels = px;
}


Screen::~Screen()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

bool Screen::TogglePixel(unsigned short x, unsigned short y)
{
	const unsigned int offset = (ScreenWidth * 4 * y) + x * 4;
	if (pixels[offset + 0] == 0)
	{
		pixels[offset + 0] = 255;        // b
		pixels[offset + 1] = 255;        // g
		pixels[offset + 2] = 255;        // r
		pixels[offset + 3] = SDL_ALPHA_OPAQUE;    // a
		return false;
	}
	else
	{
		pixels[offset + 0] = 0;        // b
		pixels[offset + 1] = 0;        // g
		pixels[offset + 2] = 0;        // r
		pixels[offset + 3] = SDL_ALPHA_OPAQUE;    // a
		return true;
	}

}

void Screen::Clear()
{
	for (auto &p : pixels)
	{
		p = 0;
	}
}

void Screen::DrawFrame()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	SDL_UpdateTexture
		(
			texture,
			NULL,
			&pixels[0],
			ScreenWidth * 4
			);

	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}
