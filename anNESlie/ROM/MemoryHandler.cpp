#include "MemoryHandler.h"

using namespace ROM;

MemoryHandler::MemoryHandler(char* memory_adress, int memory_size)
{
	memory = memory_adress;
	readHandlers = new std::function<char(int)>[memory_size];
	writeHandlers = new std::function<void(int, char)>[memory_size];

	auto read_func = [this](int adress) {return memory[adress]; };
	auto write_func = [this](int adress, char value) {memory[adress] = value; };
	for (int i = 0; i < memory_size; i++)
	{
		memory[i] = 0;
		readHandlers[i] = read_func;
		writeHandlers[i] = write_func;
	}
}

MemoryHandler::~MemoryHandler()
{
	if (readHandlers == nullptr)
		return;
	delete[] readHandlers;
	delete[] writeHandlers;
	readHandlers = nullptr;
}

inline char * ROM::MemoryHandler::GetMemoryAdress()
{
	return memory;
}

inline void ROM::MemoryHandler::SetMemoryAdress(char * memory_adress)
{
	memory = memory_adress;
}

inline void ROM::MemoryHandler::SetReadHandler(int adress, char(*handler)(int))
{
	if (handler == nullptr)
		readHandlers[adress] = [this](int adress) {return memory[adress]; };
	readHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetReadHandler(int adress, std::function<char(int)> handler)
{
	readHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetReadHandler(int adress_start, int adress_end, std::function<char(int)> handler)
{
	for (int i = adress_start; i < adress_end; i++)
		SetReadHandler(i, handler);
}

inline void ROM::MemoryHandler::SetWriteHandler(int adress, void(*handler)(int, char))
{
	if (handler == nullptr)
		writeHandlers[adress] = [this](int adress, char value) {memory[adress] = value; };
	writeHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetWriteHandler(int adress, std::function<void(int, char)>  handler)
{
	writeHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetWriteHandler(int adress_start, int adress_end, std::function<void(int, char)>  handler)
{
	for (int i = adress_start; i < adress_end; i++)
		SetWriteHandler(i, handler);
}

inline char ROM::MemoryHandler::ReadByte(int adress)
{
	if (readHandlers[adress] == nullptr)
		return memory[adress];
	else
		return readHandlers[adress](adress);
}

inline void ROM::MemoryHandler::WriteByte(int adress, char value)
{
	if (writeHandlers[adress] == nullptr)
		memory[adress] = value;
	else
		writeHandlers[adress](adress, value);
}

short ROM::MemoryHandler::ReadWord(int adress)
{
	return ((short)ReadByte(adress)) | (((short)ReadByte(adress + 1)) << 8);
}

void ROM::MemoryHandler::WriteWord(int adress, short value)
{
	WriteByte(adress, value);
	WriteByte(adress + 1, value >> 8);
}

