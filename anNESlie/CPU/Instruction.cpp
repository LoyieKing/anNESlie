#include "Core.h"


void CPU::CPUCore::JSR()
{
	PushWord(getPC() + 1);
	setPC(NextWord());
}

void CPU::CPUCore::RTI()
{
	NextByte();
	setP(Pop());
	setPC(PopWord());
}

void CPU::CPUCore::RTS()
{
	NextByte();
	setPC(PopWord() + 1);
}
void CPU::CPUCore::INY()
{
	setY(getY() + 1);
}
void CPU::CPUCore::DEY()
{
	setY(getY() - 1);
}
void CPU::CPUCore::INX()
{
	setX(getX() + 1);
}
void CPU::CPUCore::DEX()
{
	setX(getX() - 1);
}
void CPU::CPUCore::TAY()
{
	setY(getA());
}
void CPU::CPUCore::TYA()
{
	setA(getY());
}
void CPU::CPUCore::TAX()
{
	setX(getA());
}
void CPU::CPUCore::TXA()
{
	setA(getX());
}
void CPU::CPUCore::TSX()
{
	setX(getSP());
}
void CPU::CPUCore::TXS()
{
	setSP(getX());
}
void CPU::CPUCore::PHP()
{
	Push(getFlag_B());
}
void CPU::CPUCore::PLP()
{
	setP(Pop() & BreakSourceBit);
}
//TODO: complete all opcodes' def
void CPU::CPUCore::PLA()
{
	setA(Pop());
}
void CPU::CPUCore::PHA()
{
	Push(getA());
}
void CPU::CPUCore::BIT()
{
	
}
void CPU::CPUCore::JMP()
{
}
void CPU::CPUCore::BCS()
{
}
void CPU::CPUCore::BCC()
{
}
void CPU::CPUCore::BEQ()
{
}
void CPU::CPUCore::BNE()
{
}
void CPU::CPUCore::BVS()
{
}
void CPU::CPUCore::BVC()
{
}
void CPU::CPUCore::BPL()
{
}
void CPU::CPUCore::BMI()
{
}
void CPU::CPUCore::STA()
{
}
void CPU::CPUCore::STX()
{
}
void CPU::CPUCore::STY()
{
}
void CPU::CPUCore::CLC()
{
}
void CPU::CPUCore::SEC()
{
}
void CPU::CPUCore::CLI()
{
}
void CPU::CPUCore::SEI()
{
}
void CPU::CPUCore::CLV()
{
}
void CPU::CPUCore::CLD()
{
}
void CPU::CPUCore::SED()
{
}
void CPU::CPUCore::NOP()
{
}
void CPU::CPUCore::LDA()
{
}
void CPU::CPUCore::LDY()
{
}
void CPU::CPUCore::LDX()
{
}
void CPU::CPUCore::ORA()
{
}
void CPU::CPUCore::AND()
{
}
void CPU::CPUCore::EOR()
{
}
void CPU::CPUCore::SBC()
{
}
void CPU::CPUCore::ADC()
{
}
void CPU::CPUCore::BRK()
{
}
void CPU::CPUCore::CMP()
{
}
void CPU::CPUCore::CPX()
{
}
void CPU::CPUCore::CPY()
{
}
void CPU::CPUCore::LSR()
{
}
void CPU::CPUCore::ASL()
{
}
void CPU::CPUCore::ROR()
{
}
void CPU::CPUCore::ROL()
{
}
void CPU::CPUCore::INC()
{
}
void CPU::CPUCore::DEC()
{
}
//unofficial opcodes
void CPU::CPUCore::SKB()
{
}
void CPU::CPUCore::ANC()
{
}
void CPU::CPUCore::ALR()
{
}
void CPU::CPUCore::ARR()
{
}
void CPU::CPUCore::ATX()
{
}

void CPU::CPUCore::Branch(bool cond)
{
}

void  CPU::CPUCore::ADCImpl(SByte value)
{
}

void  CPU::CPUCore::CMPImpl(Byte reg)
{
}