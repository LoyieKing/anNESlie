#pragma once
#include "../Types.h"

namespace PPU
{
	const int GAME_WIDTH = 256;
	const int GAME_HEIGHT = 240;

	const int SCANLINE_COUNT = 261;
	const int CYCLES_PER_LINE = 341;

	class PPU
	{
	private:
		DWord bufferPos;
		DWord rawBitmap[GAME_WIDTH * GAME_HEIGHT];
		DWord priority[GAME_WIDTH * GAME_HEIGHT];

		DWord palette[64];

		int cpuSyncCounter;
		DWord scanlineOAM[8 * 4];
		bool isSprite0[8];
		int spriteCount;

		QWord titleShiftRegister;
		DWord currentNametableByte;
		DWord currentHighTitle, currentLowTitle;
		DWord currentColor;

		int cpuClocksSinceVBL;
		int ppuClocksSinceVBL;

		void countSpritesOnLine(int scanline);
		void nextNametableByte();
		void nextTitleByte(bool hi);
		void nextAttributeByte();
		void shiftTitleRegister();
		void processBackgroundForPixel(int cycle, int scanline);
		void processSpritesForPixel(int x, int scanline);

	public:
		PPU();

		void ProcessFrame();
		void ProcessScanline();
		void ProcessCycle();
		void ProcessPixel(int x, int y);
	};


}