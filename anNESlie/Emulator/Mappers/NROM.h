#pragma once
#include "../Types.h"
#include "BaseMapper.h"

namespace Mapper
{
	class NROM :virtual public BaseMapper
	{
	private:
		Byte addressSpace[0x2000 + 0x8000]; // Space for $2000 VRAM + $8000 PRG
	public:
		NROM(Emulator* emulator);
		void InitializeMemoryMap(CPU* cpu);

		virtual ~NROM();
	};
}