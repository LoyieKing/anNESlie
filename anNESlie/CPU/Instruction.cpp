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
	Word val = AdressRead();
	setFlag_V((val & 0x40) > 0);
	setFlag_Z((val & getA()) == 0);
	setFlag_N((val & 0x80) > 0);
}
void CPU::CPUCore::JMP()
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
void CPU::CPUCore::BCS()
{
	Branch(getFlag_C());
}
void CPU::CPUCore::BCC()
{
	Branch(!getFlag_C());
}
void CPU::CPUCore::BEQ()
{
	Branch(getFlag_Z());
}
void CPU::CPUCore::BNE()
{
	Branch(!getFlag_Z());
}
void CPU::CPUCore::BVS()
{
	Branch(getFlag_V());
}
void CPU::CPUCore::BVC()
{
	Branch(!getFlag_V());
}
void CPU::CPUCore::BPL()
{
	Branch(!getFlag_N());
}
void CPU::CPUCore::BMI()
{
	Branch(getFlag_N());
}
void CPU::CPUCore::STA()
{
	AdressWrite(getA());
}
void CPU::CPUCore::STX()
{
	AdressWrite(getX());
}
void CPU::CPUCore::STY()
{
	AdressWrite(getY());
}
void CPU::CPUCore::CLC()
{
	setFlag_C(false);
}
void CPU::CPUCore::SEC()
{
	setFlag_C(true);
}
void CPU::CPUCore::CLI()
{
	setFlag_I(false);
}
void CPU::CPUCore::SEI()
{
	setFlag_I(true);
}
void CPU::CPUCore::CLV()
{
	setFlag_V(false);
}
void CPU::CPUCore::CLD()
{
	setFlag_D(false);
}
void CPU::CPUCore::SED()
{
	setFlag_D(true);
}
void CPU::CPUCore::NOP()
{

}
void CPU::CPUCore::LDA()
{
	setA(AdressRead());
}
void CPU::CPUCore::LDY()
{
	setY(AdressRead());
}
void CPU::CPUCore::LDX()
{
	setX(AdressRead());
}
void CPU::CPUCore::ORA()
{
	setA(getA() | AdressRead());
}
void CPU::CPUCore::AND()
{
	setA(getA() & AdressRead());
}
void CPU::CPUCore::EOR()
{
	setA(getA() ^ AdressRead());
}
void CPU::CPUCore::SBC()
{
	ADCImpl((Byte)~AdressRead());
}
void CPU::CPUCore::ADC()
{
	ADCImpl(AdressRead());
}
void CPU::CPUCore::BRK()
{
	NextByte();
	Push(getP() | BreakSourceBit);
	setFlag_I(true);
	setPC(ReadByte(0xFFFE) | ReadByte(0xFFFF) << 8);
}
void CPU::CPUCore::CMP()
{
	CMPImpl(getA());
}
void CPU::CPUCore::CPX()
{
	CMPImpl(getX());
}
void CPU::CPUCore::CPY()
{
	CMPImpl(getY());
}
void CPU::CPUCore::LSR()
{
	Word D = AdressRead();
	setFlag_C((D & 0x1) > 0);
	D >>= 1;
	setFlag_N_Z(D);
	AdressWrite(D);
}
void CPU::CPUCore::ASL()
{
	Word D = AdressRead();
	setFlag_C((D & 0x80) > 0);
	D <<= 1;
	setFlag_N_Z(D);
	AdressWrite(D);
}
void CPU::CPUCore::ROR()
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
void CPU::CPUCore::ROL()
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
void CPU::CPUCore::INC()
{
	Byte D = (Byte)(AdressRead() + 1);
	setFlag_N_Z(D);
	AdressWrite(D);
}
void CPU::CPUCore::DEC()
{
	Byte D = (Byte)(AdressRead() - 1);
	setFlag_N_Z(D);
	AdressWrite(D);
}
//unofficial opcodes
void CPU::CPUCore::SKB()
{
	NextByte();
}
void CPU::CPUCore::ANC()
{
	setA(getA() & AdressRead());
	setFlag_C(getFlag_N());
}
void CPU::CPUCore::ALR()
{
	setA(getA() & AdressRead());
	setFlag_C((getA() & 0x1) > 0);
	setA(getA() >> 1);
	setFlag_N_Z(getA());
}
void CPU::CPUCore::ARR()
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
void CPU::CPUCore::ATX()
{
	setA(getA() | ReadByte(0xEE));
	setA(getA() & AdressRead());
	setX(getA());
}

void CPU::CPUCore::Branch(bool cond)
{
	Byte nPC = getPC() + NextSByte() + 1;
	if (cond)
	{
		setPC(nPC);
		cycle++;
	}
}

void  CPU::CPUCore::ADCImpl(SByte value)
{
}

void  CPU::CPUCore::CMPImpl(Byte reg)
{
}