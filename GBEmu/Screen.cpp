#include "Screen.h"
#include "Gameboy.h"
#include <iostream>
using namespace std;


Screen::Screen()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = window = SDL_CreateWindow
		(
			"SDL2",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			256, 256,
			SDL_WINDOW_SHOWN
			);

	renderer = SDL_CreateRenderer
		(
			window,
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
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
	tileMap = SDL_CreateTexture
		(
			renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			128, 128
			);


	vector< unsigned char > px(texWidth * texHeight * 4, 0);
	vector< unsigned char > tilepx(128 *128* 4, 0);
	backgroundpixels = px;
	tileMapPixels = tilepx;
}


Screen::~Screen()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

bool Screen::TogglePixel(unsigned short x, unsigned short y)
{
	const unsigned int offset = (ScreenWidth * 4 * y) + x * 4;
	if (backgroundpixels[offset + 0] == 0)
	{
		backgroundpixels[offset + 0] = 255;        // b
		backgroundpixels[offset + 1] = 255;        // g
		backgroundpixels[offset + 2] = 255;        // r
		backgroundpixels[offset + 3] = SDL_ALPHA_OPAQUE;    // a
		return false;
	}
	else
	{
		backgroundpixels[offset + 0] = 0;        // b
		backgroundpixels[offset + 1] = 0;        // g
		backgroundpixels[offset + 2] = 0;        // r
		backgroundpixels[offset + 3] = SDL_ALPHA_OPAQUE;    // a
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

	
	backgroundpixels[offset + 0] = pxC;        // b
	backgroundpixels[offset + 1] = pxC;        // g
	backgroundpixels[offset + 2] = pxC;        // r
	if (color == 0)
	{
		backgroundpixels[offset + 3] = SDL_ALPHA_TRANSPARENT;    // a		
	}
	else
	{
		backgroundpixels[offset + 3] = SDL_ALPHA_OPAQUE;    // a		
	}
}

void Screen::SetPixelTilemap(unsigned short x, unsigned short y, int color)
{
	if (x < 0 || x > 255)
	{
		return;
	}
	if (y < 0 || y > 255)
	{
		return;
	}

	const unsigned int offset = (128 * 4 * y) + x * 4;
	char pxC = 0;
	switch (color)
	{
	case 0:pxC = 0; break;
	case 1:pxC = 100; break;
	case 2:pxC = 180; break;
	case 3:pxC = 255; break;

	}


	tileMapPixels[offset + 0] = pxC;        // b
	tileMapPixels[offset + 1] = pxC;        // g
	tileMapPixels[offset + 2] = pxC;        // r
	if (color == 0)
	{
		tileMapPixels[offset + 3] = SDL_ALPHA_TRANSPARENT;    // a		
	}
	else
	{
		tileMapPixels[offset + 3] = SDL_ALPHA_OPAQUE;    // a		
	}
	
}

void Screen::Clear()
{
	for (auto &p : backgroundpixels)
	{
		p = 0;
	}
}

void Screen::DrawFrame()
{
	UpdateTilemap();
	
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	SDL_UpdateTexture
		(
			tileMap,
			NULL,
			//&backgroundpixels[0],
			&tileMapPixels[0],
			128 * 4
			);

	SDL_Rect tst;
	tst.h = 16;
	tst.w = 16;
	
	tst.x =( 1 % 16)*2;
	tst.y =( 1 / 16)*2;
	DrawBackground();
	DrawSprites();
	//SDL_RenderCopy(renderer, tileMap,nullptr,nullptr);
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

void Screen::DrawTile(uint8_t tilenumber, uint32_t x, uint32_t y)
{
	SDL_Rect tst;
	tst.h = 8;
	tst.w = 8;

	tst.x = x;
	tst.y = y;

	SDL_RenderCopy(renderer, tileMap, &GetTileRect(tilenumber), &tst);
}

void Screen::DrawTilemapTile(std::vector<uint8_t> &tile, uint32_t x, uint32_t y)
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

			SetPixelTilemap(x + (7 - xi), y + yi, c);

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
	//tilemap positive X and Y
	for (int ty = 0; ty < 32; ty++)
	{
		for (int tx = 0; tx < 32; tx++)
		{
			uint8_t tilenumber = vram[(BGMapAdress - 0x8000) + tx + ty * 32];

			//std::vector<uint8_t> tile = getTile(tilenumber);

			SDL_Rect tst;
			tst.h = 8;
			tst.w = 8;

			tst.x = tx*8 + 256-scrollX;
			tst.y = ty *8 + 256-scrollY;
			SDL_RenderCopy(renderer, tileMap, &GetTileRect(tilenumber), &tst);

			tst.x -= 256;
			SDL_RenderCopy(renderer, tileMap, &GetTileRect(tilenumber), &tst);

			tst.x += 256;
			tst.y -= 256;
			SDL_RenderCopy(renderer, tileMap, &GetTileRect(tilenumber), &tst);

			tst.x -= 256;
			SDL_RenderCopy(renderer, tileMap, &GetTileRect(tilenumber), &tst);

			
			//DrawTile(tile, tx * 8, ty * 8);
		}
	}
	
	
}
std::vector<uint8_t> Screen::getTile(uint16_t tilenumber)
{
	std::vector<uint8_t> tile(16);

	for (int i = 0; i < 16; i++)
	{
		tile[i] = vram[i + tilenumber * 16];
	}

	return tile;
}

void Screen::writeByte(uint16_t address, uint8_t byte)
{
	vram[address - 0x8000] = byte;
}

SDL_Rect Screen::GetTileRect(uint8_t tilenumber)
{
	//256 tiles, 16*16 square
	SDL_Rect rect;
	rect.h = 8;
	rect.w = 8;

	rect.x = (tilenumber % 16) * 8;
	rect.y = (tilenumber / 16) * 8;

	return rect;
}

void Screen::SetMapAdress(uint16_t address)
{
	BGMapAdress = address;
	cout << "map adress is" << hex << address;
	return;
}

void Screen::UpdateTilemap()
{
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			std::vector<uint8_t> testtile(16);
			for (int i = 0; i < 16; i++)
			{
				testtile[i] = vram[i + x * 16 + y*16*16];
			}

			DrawTilemapTile(testtile, x*8, y*8);
		}
	}
}

void Screen::DrawSprites()
{
	

	for (int i = 0; i < 10; i++)
	{
		
		Sprite sprite;

		sprite.posY= vram[0xfe00 - 0x8000 + i * 4 + 0];
		sprite.posX = vram[0xfe00 - 0x8000 + i * 4 + 1];
		sprite.Tile = vram[0xfe00 - 0x8000 + i * 4 + 2];
		sprite.Flags = vram[0xfe00 - 0x8000 + i * 4 + 3];

		
		DrawSprite(sprite);
		//DrawTile(getTile(pattern),posx, posy);
	}

}

void Screen::DrawSprite(Sprite sprite)
{

	SDL_Rect tst;
	tst.h = 8;
	tst.w = 8;

	tst.x = 8 + sprite.posX - 8;
	tst.y = 8 + sprite.posY - 16;

	bool flipY = getbit(sprite.Flags, 6);
	bool flipX = getbit(sprite.Flags,5);

	int flip = SDL_FLIP_NONE;
	if (flipX && flipY)
	{
		flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
	}
	else if (flipY)
	{
		flip = SDL_FLIP_VERTICAL;
	}
	else if (flipX)
	{
		flip = SDL_FLIP_HORIZONTAL ;
	}
	SDL_SetTextureBlendMode(tileMap, SDL_BLENDMODE_BLEND);
	SDL_RenderCopyEx(renderer, tileMap, &GetTileRect(sprite.Tile), &tst, 0, nullptr, SDL_RendererFlip(flip));

	

	//DrawTile(sprite.Tile, sprite.posX, sprite.posY);
}
