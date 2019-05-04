#pragma once
#include "CPU.h"

void CPU::JSR()
{
	PushWord(getPC() + 1);
	setPC(NextWord());
}

void CPU::RTI()
{
	NextByte();
	setP(Pop());
	setPC(PopWord());
}

void CPU::RTS()
{
	NextByte();
	setPC(PopWord() + 1);
}
void CPU::INY()
{
	setY(getY() + 1);
}
void CPU::DEY()
{
	setY(getY() - 1);
}
void CPU::INX()
{
	setX(getX() + 1);
}
void CPU::DEX()
{
	setX(getX() - 1);
}
void CPU::TAY()
{
	setY(getA());
}
void CPU::TYA()
{
	setA(getY());
}
void CPU::TAX()
{
	setX(getA());
}
void CPU::TXA()
{
	setA(getX());
}
void CPU::TSX()
{
	setX(getSP());
}
void CPU::TXS()
{
	setSP(getX());
}
void CPU::PHP()
{
	Push(getFlag_B());
}
void CPU::PLP()
{
	setP(Pop() & BreakSourceBit);
}

void CPU::PLA()
{
	setA(Pop());
}
void CPU::PHA()
{
	Push(getA());
}
void CPU::BIT()
{
	Word val = AddressRead();
	setFlag_V((val & 0x40) > 0);
	setFlag_Z((val & getA()) == 0);
	setFlag_N((val & 0x80) > 0);
}
void CPU::JMP()
{
	if (!memoryAddressHasValue)
		return;
	if (currentInstruction == 0x4C)
		setPC(NextWord());
	else if (currentInstruction == 0x6C)
	{
		Word off = NextWord();

		Word hi = (off & 0xFF) == 0xFF ? off - 0xFF : off + 1;
		Word oldPC = getPC();
		setPC(ReadByte(off) | ReadByte(hi) << 8);

		if ((oldPC & 0xFF00) != (getPC() & 0xFF00))
			cycle += 2;
	}
}
void CPU::BCS()
{
	Branch(getFlag_C());
}
void CPU::BCC()
{
	Branch(!getFlag_C());
}
void CPU::BEQ()
{
	Branch(getFlag_Z());
}
void CPU::BNE()
{
	Branch(!getFlag_Z());
}
void CPU::BVS()
{
	Branch(getFlag_V());
}
void CPU::BVC()
{
	Branch(!getFlag_V());
}
void CPU::BPL()
{
	Branch(!getFlag_N());
}
void CPU::BMI()
{
	Branch(getFlag_N());
}
void CPU::STA()
{
	AddressWrite(getA());
}
void CPU::STX()
{
	AddressWrite(getX());
}
void CPU::STY()
{
	AddressWrite(getY());
}
void CPU::CLC()
{
	setFlag_C(false);
}
void CPU::SEC()
{
	setFlag_C(true);
}
void CPU::CLI()
{
	setFlag_I(false);
}
void CPU::SEI()
{
	setFlag_I(true);
}
void CPU::CLV()
{
	setFlag_V(false);
}
void CPU::CLD()
{
	setFlag_D(false);
}
void CPU::SED()
{
	setFlag_D(true);
}
void CPU::NOP()
{

}
void CPU::LDA()
{
	setA(AddressRead());
}
void CPU::LDY()
{
	setY(AddressRead());
}
void CPU::LDX()
{
	setX(AddressRead());
}
void CPU::ORA()
{
	setA(getA() | AddressRead());
}
void CPU::AND()
{
	setA(getA() & AddressRead());
}
void CPU::EOR()
{
	setA(getA() ^ AddressRead());
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
	Push(getP() | BreakSourceBit);
	setFlag_I(true);
	setPC(ReadByte(0xFFFE) | ReadByte(0xFFFF) << 8);
}
void CPU::CMP()
{
	CMPImpl(getA());
}
void CPU::CPX()
{
	CMPImpl(getX());
}
void CPU::CPY()
{
	CMPImpl(getY());
}

// HACK: Word D = AdressRead(); END LINE 262
void CPU::LSR()
{
	Byte D = AddressRead();
	setFlag_C((D & 0x1) > 0);
	D >>= 1;
	setFlag_N_Z(D);
	AddressWrite(D);
}
void CPU::ASL()
{
	Byte D = AddressRead();
	setFlag_C((D & 0x80) > 0);
	D <<= 1;
	setFlag_N_Z(D);
	AddressWrite(D);
}
void CPU::ROR()
{
	Byte D = AddressRead();
	bool c = getFlag_C();
	setFlag_C((D & 0x1) > 0);
	D >>= 1;
	if (c)
		D |= 0x80;
	setFlag_N_Z(D);
	AddressWrite(D);
}
void CPU::ROL()
{
	Byte D = AddressRead();
	bool c = getFlag_C();
	setFlag_C((D & 0x80) > 0);
	D <<= 1;
	if (c)
		D |= 0x1;
	setFlag_N_Z(D);
	AddressWrite(D);
}
void CPU::INC()
{
	Byte D = (Byte)(AddressRead() + 1);
	setFlag_N_Z(D);
	AddressWrite(D);
}
void CPU::DEC()
{
	Byte D = (Byte)(AddressRead() - 1);
	setFlag_N_Z(D);
	AddressWrite(D);
}
//unofficial opcodes
void CPU::SKB()
{
	NextByte();
}
void CPU::ANC()
{
	setA(getA() & AddressRead());
	setFlag_C(getFlag_N());
}
void CPU::ALR()
{
	setA(getA() & AddressRead());
	setFlag_C((getA() & 0x1) > 0);
	setA(getA() >> 1);
	setFlag_N_Z(getA());
}
void CPU::ARR()
{
	setA(getA() & AddressRead());
	bool c = getFlag_C();
	setFlag_C((getA() & 0x1) > 0);
	setA(getA() >> 1);
	if (c)
	{
		setA(getA() | 0x80);
	}
	setFlag_N_Z(getA());
}
void CPU::ATX()
{
	setA(getA() | ReadByte(0xEE));
	setA(getA() & AddressRead());
	setX(getA());
}

void CPU::Branch(bool cond)
{
	Word nPC = getPC() + NextSByte() + 1;
	if (cond)
	{
		setPC(nPC);
		cycle++;
	}
}

void  CPU::ADCImpl(Byte val)
{
	int nA = (SByte)registerA + (SByte)val + (SByte)(getFlag_C() ? 1 : 0);
	setFlag_V(nA < -128 || nA > 127);
	setFlag_C((registerA + val + (Byte)(getFlag_C() ? 1 : 0)) > 0xFF);
	registerA = (Byte)(nA & 0xFF);
}

void  CPU::CMPImpl(Byte reg)
{
	Byte d = reg - AddressRead();

	setFlag_N((d & 0x80) > 0 && d != 0);
	setFlag_C(d >= 0);
	setFlag_Z(d == 0);
}
