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
			256*2, 256*2,
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

void Screen::SetPixel(unsigned short x, unsigned short y, int color)
{
	if (x < 0 || x > 255)
	{
		return;
	}
	if (y < 0 || y > 255)
	{
		return;
	}

	const unsigned int offset = (ScreenWidth * 4 * y) + x * 4;
	char pxC = 0;
	switch (color)
	{
	case 0:pxC = 0; break;
	case 1:pxC = 100; break;
	case 2:pxC = 180; break;
	case 3:pxC = 255; break;
		
	}

	
	pixels[offset + 0] = pxC;        // b
	pixels[offset + 1] = pxC;        // g
	pixels[offset + 2] = pxC;        // r
	pixels[offset + 3] = SDL_ALPHA_OPAQUE;    // a		
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
	DrawBackground();
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

void Screen::DrawTile(std::vector<uint8_t> &tile, uint32_t x, uint32_t y)
{
	int yi = 0;
	while (yi < 8)
	{
		int xi = 0;
		while (xi < 8)
		{
			uint8_t bit1 = (tile[yi * 2] >> xi) & 0x1;
			uint8_t bit2 = (tile[yi * 2 + 1] >> xi) & 0x1;

			uint8_t c = bit1 + (bit2 << 1);

			SetPixel(x+ (7- xi), y+yi, c);

			xi++;
		}

		yi++;
	}
	
}

void Screen::TestDraw(unsigned char*first, unsigned int sprites)
{
	const int memoffset = 64; //64 bytes per sprite
	
	int i = 0;
	int spritedata[16];
	for (i = 0; i < 16; i++);
	{
		spritedata[i] = *(first + i);
	}

	int x, y;
	x = 0;
	y = 0;
	for (y = 0; y < 8; y++);
	{
		for (x= 0; x < 8; x++);
		{
			unsigned char bit1 = (spritedata[y * 2] >> x) & 0x1;
			unsigned char bit2 = (spritedata[y * 2 + 1] >> x) & 0x1;

			unsigned char c = bit1 + (bit2 << 1);

			SetPixel(x, y, c);
		}
	}

}

void Screen::DrawBackground()
{
	//tilemap
	for (int ty = 0; ty < 32; ty++)
	{
		for (int tx = 0; tx < 32; tx++)
		{
			uint8_t tilenumber = vram[0x1800 + tx + ty * 32];

			std::vector<uint8_t> tile(16);

			for (int i = 0; i < 16; i++)
			{
				
					tile[i] = vram[ i + tilenumber * 16];
				

			}

			DrawTile(tile, tx * 8, ty * 8);
		}
	}
}
