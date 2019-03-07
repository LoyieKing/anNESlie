#include "MemoryHandler.h"


ROM::MemoryHandler::MemoryHandler(char* memory_adress, int memory_size)
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

ROM::MemoryHandler::~MemoryHandler()
{
	if (readHandlers == nullptr)
		return;
	delete[] readHandlers;
	delete[] writeHandlers;
	readHandlers = nullptr;
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

