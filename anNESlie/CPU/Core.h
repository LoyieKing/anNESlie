#pragma once

#include "Registers.h"
#include "Memory.h"

namespace CPU
{


	class CPUCore
	{
	public:
		friend class Memory;
	private:
		Registers registers;
		Memory memory;

		/*instructions*/
		inline void JSR();
		inline void RTI();
		inline void RTS();
		inline void INY();
		inline void DEY();
		inline void INX();
		inline void DEX();
		inline void TAY();
		inline void TYA();
		inline void TAX();
		inline void TXA();
		inline void TSX();
		inline void TXS();
		inline void PHP();
		inline void PLP();
		inline void PLA();
		inline void PHA();
		inline void BIT();
		inline void JMP();
		inline void BCS();
		inline void BCC();
		inline void BEQ();
		inline void BNE();
		inline void BVS();
		inline void BVC();
		inline void BPL();
		inline void BMI();
		inline void STA();
		inline void STX();
		inline void STY();
		inline void CLC();
		inline void SEC();
		inline void CLI();
		inline void SEI();
		inline void CLV();
		inline void CLD();
		inline void SED();
		inline void NOP();
		inline void LDA();
		inline void LDY();
		inline void LDX();
		inline void ORA();
		inline void AND();
		inline void EOR();
		inline void SBC();
		inline void ADC();
		inline void BRK();
		inline void CMP();
		inline void CPX();
		inline void CPY();
		inline void LSR();
		inline void ASL();
		inline void ROR();
		inline void ROL();
		inline void INC();
		inline void DEC();
		//unofficial opcodes
		inline void SKB();
		inline void ANC();
		inline void ALR();
		inline void ARR();
		inline void ATX();

		inline void Branch(bool cond);
		inline void ADCImpl(SByte value);
		inline void CMPImpl(Byte reg);

	public:
		CPUCore();
	};

	inline void CPU::CPUCore::JSR()
	{
		memory.PushWord(registers.getPC() + 1);
		registers.setPC(memory.NextWord());
	}

	inline void CPU::CPUCore::RTI()
	{
		memory.NextByte();
		registers.setP(memory.Pop());
		registers.setPC(memory.PopWord());
	}

	inline void CPU::CPUCore::RTS()
	{
		memory.NextByte();
		registers.setPC(memory.PopWord() + 1);
	}
	inline void CPU::CPUCore::INY()
	{
		registers.setY(registers.getY() + 1);
	}
	inline void CPU::CPUCore::DEY()
	{
		registers.setY(registers.getY() - 1);
	}
	inline void CPU::CPUCore::INX()
	{
		registers.setX(registers.getX() + 1);
	}
	inline void CPU::CPUCore::DEX()
	{
		registers.setX(registers.getX() - 1);
	}
	inline void CPU::CPUCore::TAY()
	{
		registers.setY(registers.getA());
	}
	inline void CPU::CPUCore::TYA()
	{
		registers.setA(registers.getY());
	}
	inline void CPU::CPUCore::TAX()
	{
		registers.setX(registers.getA());
	}
	inline void CPU::CPUCore::TXA()
	{
		registers.setA(registers.getX());
	}
	inline void CPU::CPUCore::TSX()
	{
		registers.setX(registers.getSP());
	}
	inline void CPU::CPUCore::TXS()
	{
		registers.setSP(registers.getX());
	}
	inline void CPU::CPUCore::PHP()
	{
		memory.Push(registers.getFlag_B());
	}
	inline void CPU::CPUCore::PLP()
	{
		registers.setP(memory.Pop()& registers.BreakSourceBit);
	}
	//TODO: complete all opcodes' def
	inline void CPU::CPUCore::PLA()
	{
	}
	inline void CPU::CPUCore::PHA()
	{
	}
	inline void CPU::CPUCore::BIT()
	{
	}
	inline void CPU::CPUCore::JMP()
	{
	}
	inline void CPU::CPUCore::BCS()
	{
	}
	inline void CPU::CPUCore::BCC()
	{
	}
	inline void CPU::CPUCore::BEQ()
	{
	}
	inline void CPU::CPUCore::BNE()
	{
	}
	inline void CPU::CPUCore::BVS()
	{
	}
	inline void CPU::CPUCore::BVC()
	{
	}
	inline void CPU::CPUCore::BPL()
	{
	}
	inline void CPU::CPUCore::BMI()
	{
	}
	inline void CPU::CPUCore::STA()
	{
	}
	inline void CPU::CPUCore::STX()
	{
	}
	inline void CPU::CPUCore::STY()
	{
	}
	inline void CPU::CPUCore::CLC()
	{
	}
	inline void CPU::CPUCore::SEC()
	{
	}
	inline void CPU::CPUCore::CLI()
	{
	}
	inline void CPU::CPUCore::SEI()
	{
	}
	inline void CPU::CPUCore::CLV()
	{
	}
	inline void CPU::CPUCore::CLD()
	{
	}
	inline void CPU::CPUCore::SED()
	{
	}
	inline void CPU::CPUCore::NOP()
	{
	}
	inline void CPU::CPUCore::LDA()
	{
	}
	inline void CPU::CPUCore::LDY()
	{
	}
	inline void CPU::CPUCore::LDX()
	{
	}
	inline void CPU::CPUCore::ORA()
	{
	}
	inline void CPU::CPUCore::AND()
	{
	}
	inline void CPU::CPUCore::EOR()
	{
	}
	inline void CPU::CPUCore::SBC()
	{
	}
	inline void CPU::CPUCore::ADC()
	{
	}
	inline void CPU::CPUCore::BRK()
	{
	}
	inline void CPU::CPUCore::CMP()
	{
	}
	inline void CPU::CPUCore::CPX()
	{
	}
	inline void CPU::CPUCore::CPY()
	{
	}
	inline void CPU::CPUCore::LSR()
	{
	}
	inline void CPU::CPUCore::ASL()
	{
	}
	inline void CPU::CPUCore::ROR()
	{
	}
	inline void CPU::CPUCore::ROL()
	{
	}
	inline void CPU::CPUCore::INC()
	{
	}
	inline void CPU::CPUCore::DEC()
	{
	}
	//unofficial opcodes
	inline void CPU::CPUCore::SKB()
	{
	}
	inline void CPU::CPUCore::ANC()
	{
	}
	inline void CPU::CPUCore::ALR()
	{
	}
	inline void CPU::CPUCore::ARR()
	{
	}
	inline void CPU::CPUCore::ATX()
	{
	}

	inline void CPUCore::Branch(bool cond)
	{
	}

	inline void CPUCore::ADCImpl(SByte value)
	{
	}

	inline void CPUCore::CMPImpl(Byte reg)
	{
	}

}