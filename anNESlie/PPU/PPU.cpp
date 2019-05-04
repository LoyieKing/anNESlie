#include "PPU.h"
#include "../Emulator/Emulator.h"
#include "../Mappers/BaseMapper.h"

const DWord PPU::palette[64] = {
		0x7C7C7C, 0x0000FC, 0x0000BC, 0x4428BC, 0x940084, 0xA80020, 0xA81000, 0x881400,
		0x503000, 0x007800, 0x006800, 0x005800, 0x004058, 0x000000, 0x000000, 0x000000,
		0xBCBCBC, 0x0078F8, 0x0058F8, 0x6844FC, 0xD800CC, 0xE40058, 0xF83800, 0xE45C10,
		0xAC7C00, 0x00B800, 0x00A800, 0x00A844, 0x008888, 0x000000, 0x000000, 0x000000,
		0xF8F8F8, 0x3CBCFC, 0x6888FC, 0x9878F8, 0xF878F8, 0xF85898, 0xF87858, 0xFCA044,
		0xF8B800, 0xB8F818, 0x58D854, 0x58F898, 0x00E8D8, 0x787878, 0x000000, 0x000000,
		0xFCFCFC, 0xA4E4FC, 0xB8B8F8, 0xD8B8F8, 0xF8B8F8, 0xF8A4C0, 0xF0D0B0, 0xFCE0A8,
		0xF8D878, 0xD8F878, 0xB8F8B8, 0xB8F8D8, 0x00FCFC, 0xF8D8F8, 0x000000, 0x000000
};

PPU::PPU(Emulator*const _emulator) :
	memoryHandler(memory, 0x3FFF)
{
	emulator = _emulator;

	/*InitializeMemoryMap*/
	memoryHandler.SetReadHandler(0x2000, 0x2FFF, [this](int address) {
		return vram[emulator->GetVRAMMirror(address)];
		});
	memoryHandler.SetReadHandler(0x3000, 0x3EFF, [this](int address) {
		return vram[emulator->GetVRAMMirror(address - 0x1000)];
		});
	memoryHandler.SetReadHandler(0x3F00, 0x3FFF, [this](int address) {
		if (address == 0x3F10 || address == 0x3F14 || address == 0x3F18 || address == 0x3F0C)
			address -= 0x10;
		return paletteRAM[(address - 0x3F00) & 0x1F];
		});

	memoryHandler.SetWriteHandler(0x2000, 0x2FFF, [this](int address, Byte val) {
		vram[emulator->GetVRAMMirror(address)] = val;
		});
	memoryHandler.SetWriteHandler(0x3000, 0x3EFF, [this](int address, Byte val) {
		vram[emulator->GetVRAMMirror(address - 0x1000)] = val;
		});
	memoryHandler.SetWriteHandler(0x3F00, 0x3FFF, [this](int address, Byte val) {
		if (address == 0x3F10 || address == 0x3F14 || address == 0x3F18 || address == 0x3F0C)
			address -= 0x10;
		paletteRAM[(address - 0x3F00) & 0x1F] = val;
		});

	emulator->Mapper->InitializeMemoryMap(this);
}

void PPU::countSpritesOnLine(int scanline)
{
	spriteCount = 0;
	int height = Flag.TallSpritesEnabled ? 16 : 8;

	//0x100:oam.Length
	for (int idx = 0; idx < 0x100; idx += 4)
	{
		int y = oam[idx] + 1;
		if (scanline >= y && scanline < y + height)
		{
			isSprite0[spriteCount] = idx == 0;
			scanlineOAM[spriteCount * 4 + 0] = oam[idx + 0];
			scanlineOAM[spriteCount * 4 + 1] = oam[idx + 1];
			scanlineOAM[spriteCount * 4 + 2] = oam[idx + 2];
			scanlineOAM[spriteCount * 4 + 3] = oam[idx + 3];
			spriteCount++;
		}

		if (spriteCount == 8)
			break;
	}
}

void PPU::nextNametableByte()
{
	currentNametableByte = ReadByte(0x2000 | (getV() & 0x0FFF));
}

void PPU::nextTileByte(bool hi)
{
	Word titileIdx = ((Word)currentNametableByte) * 16;
	Word address = Flag.PatternTableAddress + titileIdx + getFineY();

	if (hi)
		currentHighTile = ReadByte(address + 8);
	else
		currentLowTile = ReadByte(address);
}

void PPU::nextAttributeByte()
{
	Word V = getV();
	Word address = 0x23C0 | (V & 0x0C00) | ((V >> 4) & 0x38) | ((V >> 2) & 0x07);
	currentColor = (ReadByte(address) >> (DWord)((getCoarseX() & 2) | ((getCoarseY() & 2) << 1))) & 0x3;
}

void PPU::shiftTileRegister()
{
	for (int x = 0; x < 8; x++)
	{
		Word palette = ((currentHighTile & 0x80) >> 6) | ((currentLowTile & 0x80) >> 7);
		tileShiftRegister |= (palette + currentColor * 4) << ((7 - x) * 4);
		currentLowTile <<= 1;
		currentHighTile <<= 1;
	}
}

void PPU::processBackgroundForPixel(int cycle, int scanline)
{
	if (cycle < 8 && !Flag.DrawLeftBackground || !Flag.DrawBackground && scanline != -1)
	{
		// Maximally sketchy: if current address is in the PPU palette, then it draws that palette entry if rendering is disabled
		// Otherwise, it draws $3F00 (universal bg color)
		// https://www.romhacking.net/forum/index.php?topic=20554.0
		// Don't know if any game actually uses it, but a test ROM I wrote unexpectedly showed this
		// corner case
		rawBitmap[bufferPos] = palette[ReadByte(0x3F00 + ((Flag.getBusAddress() & 0x3F00) == 0x3F00 ? Flag.getBusAddress() & 0x001F : 0)) & 0x3F];
		return;
	}

	Word paletteEntry = (DWord)(tileShiftRegister >> 32 >> (int)((7 - X) * 4)) & 0x0F;
	if (paletteEntry % 4 == 0) paletteEntry = 0;

	if (scanline != -1)
	{
		priority[bufferPos] = paletteEntry;
		rawBitmap[bufferPos] = palette[ReadByte(0x3F00u + paletteEntry) & 0x3F];
	}
}

void PPU::processSpritesForPixel(int x, int scanline)
{

	for (int idx = spriteCount * 4 - 4; idx >= 0; idx -= 4)
	{
		DWord spriteX = scanlineOAM[idx + 3];
		DWord spriteY = scanlineOAM[idx] + 1;

		// Don't draw this sprite iFlag...
		if (spriteY == 0 || // it's located at y = 0
			spriteY > 239 || // it's located past y = 239 ($EF)
			x >= spriteX + 8 || // it's behind the current dot
			x < spriteX || // it's ahead of the current dot
			x < 8 && !Flag.DrawLeftSprites) // it's in the clip area, and clipping is enabled
			continue;

		// amusingly enough, the PPU's palette handling is basically identical
		// to that of the Gameboy / Gameboy Color, so I've sort of just copy/pasted
		// handling code wholesale from my GBC emulator at
		// https://github.com/Xyene/Nitrous-Emulator/blob/master/src/main/java/nitrous/lcd/LCD.java#L642
		DWord tileIdx = scanlineOAM[idx + 1];
		if (Flag.TallSpritesEnabled) tileIdx &= ~0x1u;
		tileIdx *= 16;

		DWord attrib = scanlineOAM[idx + 2] & 0xE3;

		DWord _palette = attrib & 0x3;
		bool front = (attrib & 0x20) == 0;
		bool flipX = (attrib & 0x40) > 0;
		bool flipY = (attrib & 0x80) > 0;

		int px = (int)(x - spriteX);
		int line = (int)(scanline - spriteY);

		DWord tableBase = Flag.TallSpritesEnabled ? (scanlineOAM[idx + 1] & 1) * 0x1000 : Flag.SpriteTableAddress;

		if (Flag.TallSpritesEnabled)
		{
			if (line >= 8)
			{
				line -= 8;
				if (!flipY)
					tileIdx += 16;
				flipY = false;
			}
			if (flipY) tileIdx += 16;
		}

		// here we handle the x and y flipping by tweaking the indices we are accessing
		int logicalX = flipX ? 7 - px : px;
		int logicalLine = flipY ? 7 - line : line;

		DWord address = (DWord)(tableBase + tileIdx + logicalLine);

		// this looks bad, but it's about as readable as it's going to get
		DWord color = (DWord)(
			(
			(
				(
					// fetch upper bit from 2nd bit plane
					ReadByte(address + 8) & (0x80 >> logicalX)
					) >> (7 - logicalX)
				) << 1 // this is the upper bit of the color number
				) |
				(
			(
				ReadByte(address) & (0x80 >> logicalX)
				) >> (7 - logicalX)
					)); // << 0, this is the lower bit of the color number

		if (color > 0)
		{
			DWord backgroundPixel = priority[bufferPos];
			// Sprite 0 hits...
			if (!(!isSprite0[idx / 4] || // do not occur on not-0 sprite
				x < 8 && !Flag.DrawLeftSprites || // or if left clipping is enabled
				backgroundPixel == 0 || // or if bg pixel is transparent
				Flag.Sprite0Hit || // or if it fired this frame already
				x == 255)) // or if x is 255, "for an obscure reason related to the pixel pipeline"
				Flag.Sprite0Hit = true;
			if (Flag.DrawBackground && (front || backgroundPixel == 0))
			{
				if (scanline != -1)
				{
					Word address = ReadByte(0x3F10 + _palette * 4 + color) & 0x3F;
					rawBitmap[bufferPos] = palette[address];
				}
			}
		}
	}
}

void PPU::ProcessFrame()
{
	for (int i = 0; i < GAME_WIDTH * GAME_HEIGHT; i++)
	{
		rawBitmap[i] = 0;
		priority[i] = 0;
	}
	bufferPos = 0;
	for (int i = -1; i < SCANLINE_COUNT; i++)
		ProcessScanline(i);
}

void PPU::ProcessScanline(int line)
{
	for (int i = 0; i < CYCLES_PER_LINE; i++)
		ProcessCycle(line, i);
}

void PPU::ProcessCycle(int scanline, int cycle)
{
	bool visibleCycle = 1 <= cycle && cycle <= 256;
	bool prefetchCycle = 321 <= cycle && cycle <= 336;
	bool fetchCycle = visibleCycle || prefetchCycle;

	if (Flag.VBlankStarted) ppuClocksSinceVBL++;

	if (0 <= scanline && scanline < 240 || scanline == -1)
	{
		if (visibleCycle)
			ProcessPixel(cycle - 1, scanline);

		// During pixels 280 through 304 of this scanline, the vertical scroll bits are reloaded TODO: if rendering is enabled.
		if (scanline == -1 && 280 <= cycle && cycle <= 304)
			ReloadScrollY();

		if (fetchCycle)
		{
			tileShiftRegister <<= 4;

			// See https://wiki.nesdev.com/w/images/d/d1/Ntsc_timing.png
			// Takes 8 cycles for tile to be read, 2 per "step"
			switch (cycle & 7)
			{
			case 1: // NT
				nextNametableByte();
				break;
			case 3: // AT
				nextAttributeByte();
				break;
			case 5: // Tile low
				nextTileByte(false);
				break;
			case 7: // Tile high
				nextTileByte(true);
				break;
			case 0: // 2nd cycle of tile high fetch
				if (cycle == 256)
					IncrementScrollY();
				else
					IncrementScrollX();
				// Begin rendering a brand new tile
				shiftTileRegister();
				break;
			}
		}

		if (cycle == 257)
		{
			ReloadScrollX();
			// 257 - 320
			// The tile data for the sprites on the next scanline are fetched here.
			// HACK: stagger this over all the cycles as opposed to only on 257
			countSpritesOnLine(scanline + 1);
		}
	}

	// HACK: this is a hack; VBlank should be cleared on dot 1 of the pre-render line,
	// but for some reason we're at 2272-2273 CPU clocks at that time
	// (i.e., our PPU timing is off somewhere by 6-9 PPU cycles per frame)
	if (Flag.VBlankStarted && cpuClocksSinceVBL == 2270)
	{
		Flag.VBlankStarted = false;
		cpuClocksSinceVBL = 0;
	}

	if (cycle == 1)
	{
		if (scanline == 241)
		{
			Flag.VBlankStarted = true;
			if (Flag.NMIEnabled)
				emulator->TriggerInterrupt(InterruptType::NMI);
		}

		// Happens at the same time as 1st cycle of NT byte fetch
		if (scanline == -1)
		{
			// Console.WriteLine(_ppuClocksSinceVBL);
			ppuClocksSinceVBL = 0;
			Flag.VBlankStarted = false;
			Flag.Sprite0Hit = false;
			Flag.SpriteOverflow = false;
		}
	}

	emulator->MapperProcessCycle(scanline, cycle);

	if (cpuSyncCounter + 1 == 3)
	{
		if (Flag.VBlankStarted) cpuClocksSinceVBL++;
		emulator->TickFromPPU();
		cpuSyncCounter = 0;
	}
	else cpuSyncCounter++;
}

void PPU::ProcessPixel(int x, int y)
{
	processBackgroundForPixel(x, y);
	if (Flag.DrawSprites)
		processSpritesForPixel(x, y);

	if (y != -1) bufferPos++;
}
