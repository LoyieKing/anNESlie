#pragma once

#include"Registers.h"
#include "Memory.h"

namespace CPU
{
	class Core
	{
	public:
		friend class Memory;
	private:
		Registers registers;
		Memory memory;

	public:
		Core();
	};
}