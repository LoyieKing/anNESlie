#include "Core.h"


void CPU::CPUCore::ResetInstructionAddressingMode()
{
	memoryAddressHasValue = false;
}

Word CPU::CPUCore::Adress()
{
	Word tword;
	Word tword2;

	switch (opcodes[currentInstruction].Mode)
	{
	case Immediate:
		tword = getPC(); setPC(tword + 1); return tword;
	case ZeroPage:
		return NextByte();
	case Absolute:
		return NextWord();
	case ZeroPageX:
		return (NextByte() + getX()) & 0xFF;
	case ZeroPageY:
		return (NextByte() + getY()) & 0xFF;
	case AbsoluteX:
		tword = NextWord();
		if (opcodes[currentInstruction].PageBoundary &&
			(tword & 0xFF00) != ((tword + getX()) & 0xFF00))
			cycle += 1;
		return tword + getX();
	case AbsoluteY:
		tword = NextWord();
		if (opcodes[currentInstruction].PageBoundary &&
			(tword & 0xFF00) != ((tword + getY()) & 0xFF00))
			cycle += 1;
		return tword + getY();
	case IndirectX:
		tword = (NextByte() + getX()) & 0xFF;
		return ReadByte(tword) | (ReadByte((tword + 1) & 0xFF) << 8);
	case IndirectY:
		tword = NextByte() & 0xFF;
		tword2 = ReadByte(tword) | (ReadByte((tword + 1) & 0xFF) << 8);
		if (opcodes[currentInstruction].PageBoundary &&
			(tword2 & 0xFF00) != ((tword2 + getY()) & 0xFF00))
			cycle += 1;
		return (tword2 + getY()) & 0xFFFF;
	}
	return NULL;
}

Word CPU::CPUCore::AdressRead()
{
	if (opcodes[currentInstruction].Mode == Direct)
		return rmwValue = getA();
	if (!memoryAddressHasValue)
	{
		memoryAddressHasValue = true;
		currentMemoryAddress = Adress();
	}
	return rmwValue = ReadByte(currentMemoryAddress);
}

void CPU::CPUCore::AdressWrite(Word val)
{
	if (opcodes[currentInstruction].Mode == Direct)
	{
		setA(val);
		return;
	}
	if (!memoryAddressHasValue) 
	{
		memoryAddressHasValue = true;
		currentMemoryAddress = Adress();
	}
	if (opcodes[currentInstruction].RMW)
		WriteByte(currentMemoryAddress, rmwValue);
	WriteByte(currentMemoryAddress, val);
}

Byte CPU::CPUCore::ReadByte(Word address)
{
	return memoryHandler.ReadByte(address);
}

void CPU::CPUCore::WriteByte(Word address, Byte value)
{
	memoryHandler.WriteByte(address, value);
}

Byte CPU::CPUCore::ReadWord(Word address)
{
	return memoryHandler.ReadWord(address);
}

void CPU::CPUCore::WriteWord(Word address, Byte value)
{
	memoryHandler.WriteWord(address, value);
}

Byte CPU::CPUCore::NextByte()
{
	Byte pc = getPC();
	Byte result = memoryHandler.ReadByte(pc++);
	setPC(pc);
	return result;
}

Word CPU::CPUCore::NextWord()
{
	Word result = NextByte() | ((Word)NextByte() << 8);
	return result;
}

SByte CPU::CPUCore::NextSByte()
{
	return (SByte)NextByte();
}

void CPU::CPUCore::Push(Byte value)
{
	Byte sp = getSP();
	memoryHandler.WriteByte(0x100 + sp, value);
	setSP(sp - 1);
}

Byte CPU::CPUCore::Pop()
{
	Byte sp = getSP() + 1;
	setSP(sp);
	return memoryHandler.ReadByte(sp);
}

void CPU::CPUCore::PushWord(Word value)
{
	Push(value >> 8);
	Push(value & 0xFF);
}

Word CPU::CPUCore::PopWord()
{
	Word result = Pop() | (Pop() << 8);
	return result;
}
