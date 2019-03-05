#include "MemoryHandler.h"

using namespace ROM;

MemoryHandler::MemoryHandler(char* memory_adress, int memory_size)
{
	memory = memory_adress;
	readHandlers = new ReadHandler[memory_size];
	writeHandlers = new WriteHandler[memory_size];
	for (int i = 0; i < memory_size; i++)
	{
		memory = 0;
		readHandlers = nullptr;
		writeHandlers = nullptr;
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

inline void ROM::MemoryHandler::SetReadHandler(int adress, ReadHandler handler)
{
	readHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetReadHandler(int adress_start, int adress_end, ReadHandler handler)
{
	for (int i = adress_start; i < adress_end; i++)
		SetReadHandler(i, handler);
}

inline void ROM::MemoryHandler::SetWriteHandler(int adress, WriteHandler handler)
{
	writeHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetWriteHandler(int adress_start, int adress_end, WriteHandler handler)
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

