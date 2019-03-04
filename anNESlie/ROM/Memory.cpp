#include "Memory.h"

using namespace ROM;

Memory::Memory(int memory_size)
{
	memory = new char[memory_size];
	readHandlers = new ReadHandler[memory_size];
	writeHandlers = new WriteHandler[memory_size];
	for (int i = 0; i < memory_size; i++)
	{
		memory = 0;
		readHandlers = nullptr;
		writeHandlers = nullptr;
	}
}

Memory::~Memory()
{
	if (memory == nullptr)
		return;
	delete[] memory;
	delete[] readHandlers;
	delete[] writeHandlers;
	memory = nullptr;
}

