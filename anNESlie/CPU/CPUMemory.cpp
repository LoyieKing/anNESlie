#pragma once
#include "CPU.h"

void CPU::ResetInstructionAddressingMode()
{
	memoryAddressHasValue = false;
}

Word CPU::Address()
{
	Word tword;
	Word tword2;

	switch (opcodes[currentInstruction].Mode)
	{
	case Immediate:
		return Register.PC++;
	case ZeroPage:
		return NextByte();
	case Absolute:
		return NextWord();
	case ZeroPageX:
		return (NextByte() + Register.getX()) & 0xFF;
	case ZeroPageY:
		return (NextByte() + Register.getY()) & 0xFF;
	case AbsoluteX:
		tword = NextWord();
		if (opcodes[currentInstruction].PageBoundary &&
			(tword & 0xFF00) != ((tword + Register.getX()) & 0xFF00))
			cycle += 1;
		return tword + Register.getX();
	case AbsoluteY:
		tword = NextWord();
		if (opcodes[currentInstruction].PageBoundary &&
			(tword & 0xFF00) != ((tword + Register.getY()) & 0xFF00))
			cycle += 1;
		return tword + Register.getY();
	case IndirectX:
		tword = (NextByte() + Register.getX()) & 0xFF;
		return ReadByte(tword) | (ReadByte((tword + 1) & 0xFF) << 8);
	case IndirectY:
		tword = NextByte() & 0xFF;
		tword2 = ReadByte(tword) | (ReadByte((tword + 1) & 0xFF) << 8);
		if (opcodes[currentInstruction].PageBoundary &&
			(tword2 & 0xFF00) != ((tword2 + Register.getY()) & 0xFF00))
			cycle += 1;
		return (tword2 + Register.getY()) & 0xFFFF;
	}
	return NULL;
}

Byte CPU::AddressRead()
{
	if (opcodes[currentInstruction].Mode == Direct)
		return rmwValue = Register.getA();
	if (!memoryAddressHasValue)
	{
		memoryAddressHasValue = true;
		currentMemoryAddress = Address();
	}
	return rmwValue = ReadByte(currentMemoryAddress);
}

void CPU::AddressWrite(Byte val)
{
	if (opcodes[currentInstruction].Mode == Direct)
	{
		Register.setA(val);
		return;
	}
	if (!memoryAddressHasValue)
	{
		memoryAddressHasValue = true;
		currentMemoryAddress = Address();
	}
	if (opcodes[currentInstruction].RMW)
		WriteByte(currentMemoryAddress, rmwValue);
	WriteByte(currentMemoryAddress, val);
}

Byte CPU::ReadByte(Word address)
{
	//char log[256];
	Byte val = memoryHandler.ReadByte(address);
	//sprintf(log, "Read Byte from 0x%04x, value is 0x%02x.", address, val);
	//logs.push_back(log);
	return val;
}

void CPU::WriteByte(Word address, Byte value)
{
	//char log[256];
	//sprintf(log, "Write Byte 0x%02x to 0x%04x.", value, address);
	//logs.push_back(log);
	memoryHandler.WriteByte(address, value);
}

Word CPU::ReadWord(Word address)
{
	//char log[256];
	Word val = memoryHandler.ReadWord(address);
	//sprintf(log, "Read Word from 0x%04x, value is 0x%04x.", address, val);
	//logs.push_back(log);
	return val;
}

Byte CPU::NextByte()
{
	return ReadByte(Register.PC++);
}

Word CPU::NextWord()
{
	Word result = NextByte() | ((Word)NextByte() << 8);
	return result;
}

SByte CPU::NextSByte()
{
	return (SByte)NextByte();
}

void CPU::Push(Byte value)
{
	WriteByte(0x100 + Register.SP, value);
	Register.SP--;
}

Byte CPU::Pop()
{
	Register.SP++;
	return ReadByte(0x100 + Register.SP);
}

void CPU::PushWord(Word value)
{
	Push(value >> 8);
	Push(value & 0xFF);
}

Word CPU::PopWord()
{
	Word result = Pop() | (Pop() << 8);
	return result;
}
