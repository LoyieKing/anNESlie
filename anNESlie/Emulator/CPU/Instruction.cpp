#pragma once
#include "CPU.h"

void CPU::JSR()
{
	PushWord(Register.PC + 1);
	Register.PC = NextWord();
}

void CPU::RTI()
{
	NextByte();
	Register.setP(Pop());
	Register.PC = PopWord();
}

void CPU::RTS()
{
	NextByte();
	Register.PC = PopWord() + 1;
}
void CPU::INY()
{
	Register.setY(Register.getY() + 1);
}
void CPU::DEY()
{
	Register.setY(Register.getY() - 1);
}
void CPU::INX()
{
	Register.setX(Register.getX() + 1);
}
void CPU::DEX()
{
	Register.setX(Register.getX() - 1);
}
void CPU::TAY()
{
	Register.setY(Register.getA());
}
void CPU::TYA()
{
	Register.setA(Register.getY());
}
void CPU::TAX()
{
	Register.setX(Register.getA());
}
void CPU::TXA()
{
	Register.setA(Register.getX());
}
void CPU::TSX()
{
	Register.setX(Register.SP);
}
void CPU::TXS()
{
	Register.SP = Register.getX();
}
void CPU::PHP()
{
	Push(Register.P.BreakSource ? Register.BreakSourceBit : 0);
}
void CPU::PLP()
{
	Register.setP(Pop() & ~Register.BreakSourceBit);
}

void CPU::PLA()
{
	Register.setA(Pop());
}
void CPU::PHA()
{
	Push(Register.getA());
}
void CPU::BIT()
{
	Word val = AddressRead();
	Register.P.Overflow = ((val & 0x40) > 0);
	Register.P.Zero = ((val & Register.getA()) == 0);
	Register.P.Negative = ((val & 0x80) > 0);
}
void CPU::JMP()
{
	if (currentInstruction == 0x4C)
		Register.PC = NextWord();
	else if (currentInstruction == 0x6C)
	{
		Word off = NextWord();

		Word hi = (off & 0xFF) == 0xFF ? off - 0xFF : off + 1;
		Word oldPC = Register.PC;
		Register.PC = (ReadByte(off) | ReadByte(hi) << 8);

		if ((oldPC & 0xFF00) != (Register.PC & 0xFF00))
			cycle += 2;
	}
}
void CPU::BCS()
{
	
	Branch(Register.P.Carry);
}
void CPU::BCC()
{
	Branch(!Register.P.Carry);
}
void CPU::BEQ()
{
	Branch(Register.P.Zero);
}
void CPU::BNE()
{
	Branch(!Register.P.Zero);
}
void CPU::BVS()
{
	Branch(Register.P.Overflow);
}
void CPU::BVC()
{
	Branch(!Register.P.Overflow);
}
void CPU::BPL()
{
	Branch(!Register.P.Negative);
}
void CPU::BMI()
{
	Branch(Register.P.Negative);
}
void CPU::STA()
{
	AddressWrite(Register.getA());
}
void CPU::STX()
{
	AddressWrite(Register.getX());
}
void CPU::STY()
{
	AddressWrite(Register.getY());
}
void CPU::CLC()
{
	Register.P.Carry = false;
}
void CPU::SEC()
{
	Register.P.Carry = true;
}
void CPU::CLI()
{
	Register.P.InterruptDisabled = false;
}
void CPU::SEI()
{
	Register.P.InterruptDisabled = true;
}
void CPU::CLV()
{
	Register.P.Overflow = false;
}
void CPU::CLD()
{
	Register.P.DecimalMode = false;
}
void CPU::SED()
{
	Register.P.DecimalMode = true;
}
void CPU::NOP()
{

}
void CPU::LDA()
{
	Register.setA(AddressRead());
}
void CPU::LDY()
{
	Register.setY(AddressRead());
}
void CPU::LDX()
{
	Register.setX(AddressRead());
}
void CPU::ORA()
{
	Register.setA(Register.getA() | AddressRead());
}
void CPU::AND()
{
	Register.setA(Register.getA() & AddressRead());
}
void CPU::EOR()
{
	Register.setA(Register.getA() ^ AddressRead());
}
void CPU::SBC()
{
	ADCImpl((Byte)~AddressRead());
}
void CPU::ADC()
{
	ADCImpl(AddressRead());
}
void CPU::BRK()
{
	NextByte();
	Push( Register.getP() | Register.BreakSourceBit);
	Register.P.InterruptDisabled = true;
	Register.PC = ReadByte(0xFFFE) | ReadByte(0xFFFF) << 8;
}
void CPU::CMP()
{
	CMPImpl(Register.getA());
}
void CPU::CPX()
{
	CMPImpl(Register.getX());
}
void CPU::CPY()
{
	CMPImpl(Register.getY());
}

// HACK: Word D = AdressRead(); END LINE 262
void CPU::LSR()
{
	Byte D = AddressRead();
	Register.P.Carry = (D & 0x1) > 0;
	D >>= 1;
	Register.setFlagNZ(D);
	AddressWrite(D);
}
void CPU::ASL()
{
	Byte D = AddressRead();
	Register.P.Carry = (D & 0x80) > 0;
	D <<= 1;
	Register.setFlagNZ(D);
	AddressWrite(D);
}
void CPU::ROR()
{
	Byte D = AddressRead();
	bool c = Register.P.Carry;
	Register.P.Carry = (D & 0x1) > 0;
	D >>= 1;
	if (c)
		D |= 0x80;
	Register.setFlagNZ(D);
	AddressWrite(D);
}
void CPU::ROL()
{
	Byte D = AddressRead();
	bool c = Register.P.Carry;
	Register.P.Carry = (D & 0x80) > 0;
	D <<= 1;
	if (c)
		D |= 0x1;
	Register.setFlagNZ(D);
	AddressWrite(D);
}
void CPU::INC()
{
	Byte D = (Byte)(AddressRead() + 1);
	Register.setFlagNZ(D);
	AddressWrite(D);
}
void CPU::DEC()
{
	Byte D = (Byte)(AddressRead() - 1);
	Register.setFlagNZ(D);
	AddressWrite(D);
}
//unofficial opcodes
void CPU::SKB()
{
	NextByte();
}
void CPU::ANC()
{
	Register.setA(Register.getA() & AddressRead());
	Register.P.Carry = Register.P.Negative;
}
void CPU::ALR()
{
	Register.setA(Register.getA() & AddressRead());
	Register.P.Carry = (Register.getA() & 0x1) > 0;
	Register.setA(Register.getA() >> 1);
	Register.setFlagNZ(Register.getA());
}
void CPU::ARR()
{
	Register.setA(Register.getA() & AddressRead());
	bool c = Register.P.Carry;
	Register.P.Carry = (Register.getA() & 0x1) > 0;
	Register.setA(Register.getA() >> 1);
	if (c)
	{
		Register.setA(Register.getA() | 0x80);
	}
	Register.setFlagNZ(Register.getA());
}
void CPU::ATX()
{
	Register.setA(Register.getA() | ReadByte(0xEE));
	Register.setA(Register.getA() & AddressRead());
	Register.setX(Register.getA());
}

void CPU::Branch(bool cond)
{
	// WARN: DO NOT! use NextByte() here, it will cause a compile error
	Word nPC = Register.PC + (SByte)ReadByte(Register.PC) + 1;
	Register.PC++;
	if (cond)
	{
		Register.PC = nPC;
		cycle++;
	}
}

void  CPU::ADCImpl(Byte val)
{
	int nA = (SByte)Register.getA() + (SByte)val + (SByte)(Register.P.Carry ? 1 : 0);
	Register.P.Overflow = nA < -128 || nA > 127;
	Register.P.Carry = (Register.getA() + val + (Byte)(Register.P.Carry ? 1 : 0)) > 0xFF;
	Register.setA(nA & 0xFF);
}

void  CPU::CMPImpl(Byte reg)
{
	short d = reg - AddressRead();

	Register.P.Negative = (d & 0x80) > 0 && d != 0;
	Register.P.Carry = d >= 0;
	Register.P.Zero = d == 0;
}
