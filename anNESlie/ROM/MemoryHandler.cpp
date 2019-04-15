#include "MemoryHandler.h"


ROM::MemoryHandler::MemoryHandler(Byte* memory_adress, int memory_size)
{
	memory = memory_adress;
	readHandlers = new std::function<Byte(int)>[memory_size];
	writeHandlers = new std::function<void(int, Byte)>[memory_size];

	auto read_func = [this](int adress) {return memory[adress]; };
	auto write_func = [this](int adress, Byte value) {memory[adress] = value; };
	for (int i = 0; i < memory_size; i++)
	{
		memory[i] = 0;
		readHandlers[i] = read_func;
		writeHandlers[i] = write_func;
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
	if (handler == nullptr)
		readHandlers[adress] = [this](int adress) {return memory[adress]; };
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
	if (handler == nullptr)
		writeHandlers[adress] = [this](int adress, Byte value) {memory[adress] = value; };
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
		return memory[adress];
	else
		return readHandlers[adress](adress);
}

void ROM::MemoryHandler::WriteByte(Word adress, Byte value)
{
	if (writeHandlers[adress] == nullptr)
		memory[adress] = value;
	else
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

