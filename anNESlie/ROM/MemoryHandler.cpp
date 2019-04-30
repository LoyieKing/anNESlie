#include "MemoryHandler.h"


ROM::MemoryHandler::MemoryHandler(Byte* memory_adress, int memory_size)
{
	memory = memory_adress;
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


Byte* ROM::MemoryHandler::GetMemoryAdress()
{
	return memory;
}

void ROM::MemoryHandler::SetMemoryAdress(Byte* memory_adress)
{
	memory = memory_adress;
}

void ROM::MemoryHandler::SetReadHandler(Word adress, Byte(*handler)(int))
{
	readHandlers[adress] = handler;
}

void ROM::MemoryHandler::SetReadHandler(Word adress, std::function<Byte(int)> handler)
{
	readHandlers[adress] = handler;
}

void ROM::MemoryHandler::SetReadHandler(Word adress_start, int adress_end, std::function<Byte(int)> handler)
{
	for (int i = adress_start; i < adress_end; i++)
		SetReadHandler(i, handler);
}

void ROM::MemoryHandler::SetWriteHandler(Word adress, void(*handler)(int, Byte))
{
	writeHandlers[adress] = handler;
}

void ROM::MemoryHandler::SetWriteHandler(Word adress, std::function<void(int, Byte)>  handler)
{
	writeHandlers[adress] = handler;
}

void ROM::MemoryHandler::SetWriteHandler(Word adress_start, int adress_end, std::function<void(int, Byte)>  handler)
{
	for (int i = adress_start; i < adress_end; i++)
		SetWriteHandler(i, handler);
}

Byte ROM::MemoryHandler::ReadByte(Word adress)
{
	if (readHandlers[adress] == nullptr)
		return 0;
	return readHandlers[adress](adress);
}

void ROM::MemoryHandler::WriteByte(Word adress, Byte value)
{
	if (writeHandlers[adress] == nullptr)
		return;
	writeHandlers[adress](adress, value);
}


Byte ROM::MemoryHandler::ReadWord(Word adress)
{
	return ((short)ReadByte(adress)) | (((short)ReadByte(adress + 1)) << 8);
}

void ROM::MemoryHandler::WriteWord(Word adress, Word value)
{
	WriteByte(adress, value);
	WriteByte(adress + 1, value >> 8);
}

