#pragma once

#include <SDL.h>
#include <SDL_render.h>
#include <vector>
const unsigned short ScreenWidth = 256;
const unsigned short ScreenHeight = 256;

struct Sprite
{
	uint8_t posX;
	uint8_t posY;
	uint8_t Tile;
	uint8_t Flags;
	
};



class Screen
{
public:
	Screen();
	~Screen();

	bool TogglePixel(unsigned short x, unsigned short y);
	void SetPixel(unsigned short x, unsigned short y, int color);
	void SetPixelTilemap(unsigned short x, unsigned short y, int color);
	void Clear();
	void DrawFrame();
	void DrawTile(std::vector<uint8_t> &tile, uint32_t x, uint32_t y);
	void DrawTile(uint8_t tilenumber, uint32_t x, uint32_t y);
	void DrawTilemapTile(std::vector<uint8_t> &tile, uint32_t x, uint32_t y);
	void TestDraw(unsigned char * first, unsigned int sprites);
	
	void SetScrollX(uint8_t scrollx) { scrollX = scrollx; };
	void SetScrollY(uint8_t scrolly) { scrollY = scrolly; };

	void DrawBackground();
	void DrawSprites();

	void DrawSprite(Sprite sprite);

	std::vector<uint8_t> getTile(uint16_t tilenumber);

	void writeByte(uint16_t address, uint8_t byte);

	uint8_t vram[0xF000];

	SDL_Rect GetTileRect(uint8_t tilenumber);
	void SetMapAdress(uint16_t address);
	void UpdateTilemap();

	uint16_t BGMapAdress;

private:
	uint8_t scrollX;
	uint8_t scrollY;
	SDL_Texture * tileMap;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	std::vector< uint8_t > backgroundpixels;
	std::vector< uint8_t > tileMapPixels;


};

