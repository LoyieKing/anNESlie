#pragma once

#include "..\ROM\MemoryHandler.h"
#include "..\Types.h"
#include "Core.h"

namespace CPU
{
	const int CPU_MEMORY_SIZE = 0xFFFF;

	class Memory
	{
	private:
		Byte* memory;
		ROM::MemoryHandler memoryHandler;
		Core *cpu;

	public:
		Memory(Core* _cpu);
		~Memory();

		inline Byte NextByte();
		inline Word NextWord();
		inline SByte NextSByte();

		inline void Push(Byte value);
		inline Byte Pop();
		inline void PushWord(Word value);
		inline Word PopWord();

	};

}


inline Byte CPU::Memory::NextByte()
{
	Byte pc = cpu->registers.getPC();
	Byte result = memoryHandler.ReadByte(pc++);
	cpu->registers.setPC(pc);
	return result;
}

inline Word CPU::Memory::NextWord()
{
	Word result = NextByte() | (NextByte() << 8);
	return result;
}

inline SByte CPU::Memory::NextSByte()
{
	return (SByte)NextByte();
}

inline void CPU::Memory::Push(Byte value)
{
	Byte sp = cpu->registers.getSP();
	memoryHandler.WriteByte(0x100 + sp, value);
	cpu->registers.setSP(sp - 1);
}

inline Byte CPU::Memory::Pop()
{
	Byte sp = cpu->registers.getSP() + 1;
	cpu->registers.setSP(sp);
	return memoryHandler.ReadByte(sp);
}

inline void CPU::Memory::PushWord(Word value)
{
	Push(value >> 8);
	Push(value & 0xFF);
}

inline Word CPU::Memory::PopWord()
{
	Word result = Pop() | (Pop() << 8);
	return result;
}
