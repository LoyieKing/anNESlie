#pragma once
#include <functional>

namespace CPU
{
	enum AddressingMode
	{
		None,
		Direct,
		Immediate,
		ZeroPage,
		Absolute,
		ZeroPageX,
		ZeroPageY,
		AbsoluteX,
		AbsoluteY,
		IndirectX,
		IndirectY
	};

	struct OpcodeDefine
	{
		int Opcode;
		int Cycles = 1;
		bool PageBoundary;
		bool RMW;
		AddressingMode Mode = None;
		std::function<void(void)> action;
	};


}