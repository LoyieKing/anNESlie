#pragma once
#include "CPU.h"

Byte CPU::REGISTER::getA()
{
	return A;
}

void CPU::REGISTER::setA(Byte val)
{
	A = val;
	setFlagNZ(val);
}

Byte CPU::REGISTER::getX()
{
	return X;
}

void CPU::REGISTER::setX(Byte val)
{
	X = val;
	setFlagNZ(val);
}

Byte CPU::REGISTER::getY()
{
	return Y;
}

void CPU::REGISTER::setY(Byte val)
{
	Y = val;
	setFlagNZ(val);
}

void CPU::REGISTER::setFlagNZ(Byte val)
{
	P.Zero = val == 0;
	P.Negative = (val & 0x80) > 0;
}

void CPU::REGISTER::setP(Byte val)
{
	P.BreakSource = val & BreakSourceBit;
	P.Carry = val & CarryBit;
	P.DecimalMode = val & DecimalModeBit;
	P.InterruptDisabled = val & InterruptDisabledBit;
	P.Negative = val & NegativeBit;
	P.Overflow = val & OverflowBit;
	P.Unused = val & UnusedBit;
	P.Zero = val & ZeroBit;
}

Byte CPU::REGISTER::getP()
{
	return (
		P.BreakSource << BreakSourceBit |
		P.Carry << CarryBit |
		P.DecimalMode << DecimalModeBit |
		P.InterruptDisabled << InterruptDisabledBit |
		P.Negative << NegativeBit |
		P.Overflow << OverflowBit |
		P.Unused << UnusedBit |
		P.Zero << ZeroBit
		);
}
