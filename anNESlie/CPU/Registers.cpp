#include "Core.h"


void CPU::CPUCore::setFlag_N_Z(Byte val)
{
	if (val == 0)
		registerP |= ZeroBit;
	if (val < 0)
		registerP |= NegativeBit;
}

bool CPU::CPUCore::getFlag_N()
{
	return registerP & NegativeBit;
}

void CPU::CPUCore::setFlag_N(bool val)
{
	if (val)
		registerP |= NegativeBit;
	else
		registerP &= NegativeBit;
}

bool CPU::CPUCore::getFlag_V()
{
	return registerP & OverflowBit;
}

void CPU::CPUCore::setFlag_V(bool val)
{
	if (val)
		registerP |= OverflowBit;
	else
		registerP &= ~OverflowBit;
}

bool CPU::CPUCore::getFlag_Unused()
{
	return registerP & UnusedBit;
}

void CPU::CPUCore::setFlag_Unused(bool val)
{
	if (val)
		registerP |= UnusedBit;
	else
		registerP &= ~UnusedBit;
}

bool CPU::CPUCore::getFlag_B()
{
	return registerP & BreakSourceBit;
}

void CPU::CPUCore::setFlag_B(bool val)
{
	if (val)
		registerP |= BreakSourceBit;
	else
		registerP &= ~BreakSourceBit;
}

bool CPU::CPUCore::getFlag_D()
{
	return registerP & DecimalModeBit;
}

void CPU::CPUCore::setFlag_D(bool val)
{
	if (val)
		registerP |= DecimalModeBit;
	else
		registerP &= ~DecimalModeBit;
}

bool CPU::CPUCore::getFlag_I()
{
	return registerP & InterruptDisabledBit;
}

void CPU::CPUCore::setFlag_I(bool val)
{
	if (val)
		registerP |= InterruptDisabledBit;
	else
		registerP &= ~InterruptDisabledBit;
}

bool CPU::CPUCore::getFlag_Z()
{
	return registerP & ZeroBit;
}

void CPU::CPUCore::setFlag_Z(bool val)
{
	if (val)
		registerP |= ZeroBit;
	else
		registerP &= ~ZeroBit;
}

bool CPU::CPUCore::getFlag_C()
{
	return registerP & CarryBit;
}

void CPU::CPUCore::setFlag_C(bool val)
{
	if (val)
		registerP |= CarryBit;
	else
		registerP &= ~CarryBit;
}


Byte CPU::CPUCore::getA()
{
	return registerA;
}

void CPU::CPUCore::setA(Byte val)
{
	registerA = val;
}

Byte CPU::CPUCore::getX()
{
	return registerX;
}

void CPU::CPUCore::setX(Byte val)
{
	registerX = val;
}

Byte CPU::CPUCore::getY()
{
	return registerY;
}

void CPU::CPUCore::setY(Byte val)
{
	registerY = val;
}

Byte CPU::CPUCore::getSP()
{
	return registerSP;
}

void CPU::CPUCore::setSP(Byte val)
{
	registerSP = val;
}

Word CPU::CPUCore::getPC()
{
	return registerPC;
}

void CPU::CPUCore::setPC(Word val)
{
	registerPC = val;
}

Byte CPU::CPUCore::getP()
{
	return registerP;
}

void CPU::CPUCore::setP(Byte val)
{
	registerP = val;
}
