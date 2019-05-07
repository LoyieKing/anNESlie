#include "MemoryHandler.h"


ROM::MemoryHandler::MemoryHandler(Byte* memory_address, int memory_size)
{
	auto empty_read = [](Word)->Byte {return 0; };
	auto empty_write = [](Word, Byte) {return; };

	readHandlers = new std::function<Byte(Word)>[memory_size];
	writeHandlers = new std::function<void(Word, Byte)>[memory_size];
	for (int i = 0; i < memory_size; i++)
	{
		memory_address[i] = 0;
		readHandlers[i] = empty_read;
		writeHandlers[i] = empty_write;
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

void ROM::MemoryHandler::SetReadHandler(Word address_start, Word address_end, std::function<Byte(Word)> handler)
{
	for (int i = address_start; i <= address_end; i++)
	{
		readHandlers[i] = handler;
	}
}


void ROM::MemoryHandler::SetWriteHandler(Word address_start, Word address_end, std::function<void(Word, Byte)>  handler)
{
	for (int i = address_start; i <= address_end; i++)
	{
		writeHandlers[i] = handler;
	}
}

Byte ROM::MemoryHandler::ReadByte(Word address)
{
	Byte value = readHandlers[address](address);
	return value;
}

void ROM::MemoryHandler::WriteByte(Word address, Byte value)
{
	writeHandlers[address](address, value);
}


Word ROM::MemoryHandler::ReadWord(Word address)
{
	return ((Word)ReadByte(address)) | (((Word)ReadByte(address + 1)) << 8);
}
//
//void ROM::MemoryHandler::WriteWord(Word address, Word value)
//{
//	WriteByte(address, value);
//	WriteByte(address + 1, value >> 8);
//}
//
