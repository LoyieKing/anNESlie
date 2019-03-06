#pragma once

#include "..\ROM\MemoryHandler.h"

namespace CPU
{
	const int CPU_MEMORY_SIZE = 0xFFFF;

	class Memory
	{
	private:
		char* memory;
		ROM::MemoryHandler memoryHandler;

	public:
		Memory();
		~Memory();


	};




}