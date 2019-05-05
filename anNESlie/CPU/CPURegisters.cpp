#pragma once
#include "CPU.h"

Byte CPU::Register::getA()
{
	return A;
}

void CPU::Register::setA(Byte val)
{
	A = val;
	setFlagNZ(val);
}

Byte CPU::Register::getX()
{
	return X;
}

void CPU::Register::setX(Byte val)
{
	X = val;
	setFlagNZ(val);
}

Byte CPU::Register::getY()
{
	return Y;
}

void CPU::Register::setY(Byte val)
{
	Y = val;
	setFlagNZ(val);
}

void CPU::Register::setFlagNZ(Byte val)
{
	P.Zero = val == 0;
	P.Negative = (val & 0x80) > 0;
}

void CPU::Register::setP(Byte val)
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

Byte CPU::Register::getP()
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
