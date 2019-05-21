#pragma once
#include <functional>


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

struct OpcodeDefinition
{
	int Opcode;
	int Cycles = 1;
	bool PageBoundary;
	bool RMW;
	AddressingMode Mode = None;
	std::function<void(void)> action;
};

