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
	Word val = AdressRead();
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
	AdressWrite(getA());
}
void CPU::STX()
{
	AdressWrite(getX());
}
void CPU::STY()
{
	AdressWrite(getY());
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
	setA(AdressRead());
}
void CPU::LDY()
{
	setY(AdressRead());
}
void CPU::LDX()
{
	setX(AdressRead());
}
void CPU::ORA()
{
	setA(getA() | AdressRead());
}
void CPU::AND()
{
	setA(getA() & AdressRead());
}
void CPU::EOR()
{
	setA(getA() ^ AdressRead());
}
void CPU::SBC()
{
	ADCImpl((Byte)~AdressRead());
}
void CPU::ADC()
{
	ADCImpl(AdressRead());
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
void CPU::LSR()
{
	Word D = AdressRead();
	setFlag_C((D & 0x1) > 0);
	D >>= 1;
	setFlag_N_Z(D);
	AdressWrite(D);
}
void CPU::ASL()
{
	Word D = AdressRead();
	setFlag_C((D & 0x80) > 0);
	D <<= 1;
	setFlag_N_Z(D);
	AdressWrite(D);
}
void CPU::ROR()
{
	Word D = AdressRead();
	bool c = getFlag_C();
	setFlag_C((D & 0x1) > 0);
	D >>= 1;
	if (c)
		D |= 0x80;
	setFlag_N_Z(D);
	AdressWrite(D);
}
void CPU::ROL()
{
	Word D = AdressRead();
	bool c = getFlag_C();
	setFlag_C((D & 0x80) > 0);
	D <<= 1;
	if (c)
		D |= 0x1;
	setFlag_N_Z(D);
	AdressWrite(D);
}
void CPU::INC()
{
	Byte D = (Byte)(AdressRead() + 1);
	setFlag_N_Z(D);
	AdressWrite(D);
}
void CPU::DEC()
{
	Byte D = (Byte)(AdressRead() - 1);
	setFlag_N_Z(D);
	AdressWrite(D);
}
//unofficial opcodes
void CPU::SKB()
{
	NextByte();
}
void CPU::ANC()
{
	setA(getA() & AdressRead());
	setFlag_C(getFlag_N());
}
void CPU::ALR()
{
	setA(getA() & AdressRead());
	setFlag_C((getA() & 0x1) > 0);
	setA(getA() >> 1);
	setFlag_N_Z(getA());
}
void CPU::ARR()
{
	setA(getA() & AdressRead());
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
	setA(getA() & AdressRead());
	setX(getA());
}

void CPU::Branch(bool cond)
{
	Byte nPC = getPC() + NextSByte() + 1;
	if (cond)
	{
		setPC(nPC);
		cycle++;
	}
}


// TODO: Add impl
void  CPU::ADCImpl(SByte value)
{
}

void  CPU::CMPImpl(Byte reg)
{
}
