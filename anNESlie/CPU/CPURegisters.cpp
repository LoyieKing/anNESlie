#pragma once
#include "CPU.h"

void CPU::setFlag_N_Z(Byte val)
{
	if (val == 0)
		registerP |= ZeroBit;
	if (val < 0)
		registerP |= NegativeBit;
}

bool CPU::getFlag_N()
{
	return registerP & NegativeBit;
}

void CPU::setFlag_N(bool val)
{
	if (val)
		registerP |= NegativeBit;
	else
		registerP &= NegativeBit;
}

bool CPU::getFlag_V()
{
	return registerP & OverflowBit;
}

void CPU::setFlag_V(bool val)
{
	if (val)
		registerP |= OverflowBit;
	else
		registerP &= ~OverflowBit;
}

bool CPU::getFlag_Unused()
{
	return registerP & UnusedBit;
}

void CPU::setFlag_Unused(bool val)
{
	if (val)
		registerP |= UnusedBit;
	else
		registerP &= ~UnusedBit;
}

bool CPU::getFlag_B()
{
	return registerP & BreakSourceBit;
}

void CPU::setFlag_B(bool val)
{
	if (val)
		registerP |= BreakSourceBit;
	else
		registerP &= ~BreakSourceBit;
}

bool CPU::getFlag_D()
{
	return registerP & DecimalModeBit;
}

void CPU::setFlag_D(bool val)
{
	if (val)
		registerP |= DecimalModeBit;
	else
		registerP &= ~DecimalModeBit;
}

bool CPU::getFlag_I()
{
	return registerP & InterruptDisabledBit;
}

void CPU::setFlag_I(bool val)
{
	if (val)
		registerP |= InterruptDisabledBit;
	else
		registerP &= ~InterruptDisabledBit;
}

bool CPU::getFlag_Z()
{
	return registerP & ZeroBit;
}

void CPU::setFlag_Z(bool val)
{
	if (val)
		registerP |= ZeroBit;
	else
		registerP &= ~ZeroBit;
}

bool CPU::getFlag_C()
{
	return registerP & CarryBit;
}

void CPU::setFlag_C(bool val)
{
	if (val)
		registerP |= CarryBit;
	else
		registerP &= ~CarryBit;
}


Byte CPU::getA()
{
	return registerA;
}

void CPU::setA(Byte val)
{
	registerA = val;
}

Byte CPU::getX()
{
	return registerX;
}

void CPU::setX(Byte val)
{
	registerX = val;
}

Byte CPU::getY()
{
	return registerY;
}

void CPU::setY(Byte val)
{
	registerY = val;
}

Byte CPU::getSP()
{
	return registerSP;
}

void CPU::setSP(Byte val)
{
	registerSP = val;
}

Word CPU::getPC()
{
	return registerPC;
}

void CPU::setPC(Word val)
{
	registerPC = val;
}

Byte CPU::getP()
{
	return registerP;
}

void CPU::setP(Byte val)
{
	registerP = val;
}
