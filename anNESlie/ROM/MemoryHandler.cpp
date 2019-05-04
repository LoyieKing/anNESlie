#include "MemoryHandler.h"


ROM::MemoryHandler::MemoryHandler(Byte* memory_address, int memory_size)
{
	memory = memory_address;
	readHandlers = new std::function<Byte(int)>[memory_size];
	writeHandlers = new std::function<void(int, Byte)>[memory_size];


	for (int i = 0; i < memory_size; i++)
	{
		memory[i] = 0;
		readHandlers[i] = nullptr;
		writeHandlers[i] = nullptr;
	}
}

ROM::MemoryHandler::~MemoryHandler()
{
	if (readHandlers == nullptr)
		return;
	delete[] readHandlers;
	delete[] writeHandlers;
	readHandlers = nullptr;
}


Byte* ROM::MemoryHandler::GetMemoryAddress()
{
	return memory;
}

void ROM::MemoryHandler::SetMemoryAddress(Byte* memory_address)
{
	memory = memory_address;
}

void ROM::MemoryHandler::SetReadHandler(Word address, Byte(*handler)(int))
{
	readHandlers[address] = handler;
}

void ROM::MemoryHandler::SetReadHandler(Word address, std::function<Byte(int)> handler)
{
	readHandlers[address] = handler;
}

void ROM::MemoryHandler::SetReadHandler(Word address_start, int address_end, std::function<Byte(int)> handler)
{
	for (int i = address_start; i < address_end; i++)
		SetReadHandler(i, handler);
}

void ROM::MemoryHandler::SetWriteHandler(Word address, void(*handler)(int, Byte))
{
	writeHandlers[address] = handler;
}

void ROM::MemoryHandler::SetWriteHandler(Word address, std::function<void(int, Byte)>  handler)
{
	writeHandlers[address] = handler;
}

void ROM::MemoryHandler::SetWriteHandler(Word address_start, int address_end, std::function<void(int, Byte)>  handler)
{
	for (int i = address_start; i < address_end; i++)
		SetWriteHandler(i, handler);
}

Byte ROM::MemoryHandler::ReadByte(Word address)
{
	if (readHandlers[address] == nullptr)
		return 0;
	return readHandlers[address](address);
}

void ROM::MemoryHandler::WriteByte(Word address, Byte value)
{
	if (writeHandlers[address] == nullptr)
		return;
	writeHandlers[address](address, value);
}


Word ROM::MemoryHandler::ReadWord(Word address)
{
	return ((Word)ReadByte(address)) | (((Word)ReadByte(address + 1)) << 8);
}

void ROM::MemoryHandler::WriteWord(Word address, Word value)
{
	WriteByte(address, value);
	WriteByte(address + 1, value >> 8);
}

