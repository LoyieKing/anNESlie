/*
	��NESģ�������� - ���Ϲ������� - CPUƪ��
	http://www.it165.net/embed/html/201312/2367.html

	��NESģ�����ʼǣ�1�����ż�顷
	https://www.tuicool.com/articles/vYVraq

*/

/*
	Status Register
	7 6 5 4 3 2 1 0
	N V   B D I Z C

	-Carry Flag(C)����λ��־
		��ǰһ��ָ��ļ�����bit7(���λ)�����˽�λʱ�ᱻ��Ϊ1.
		��������16λ����ӣ����ȼ����8λ�����������������ĵ�8λ֮��Ϊ256��������255����ʱ��Ϊ0��256 - 256 = 0�������������λ�����˽�λ��C��Ϊ1.�����ڼ����8λʱ�����԰�C�е�1��Ϊ��8λ��������λ��Ҳ����˵��ͨ��C�����Խ��г���8λ�����㣬������������չ�ˡ�
		Ҳ����ͨ��SECָ��(��Ϊ1)��CLCָ����Ϊ0������ָ��ȥ��дC��

	-Zero Flag(Z)�����־
		���ǰһ��ָ������Ľ��Ϊ0������128-128�ͻ�ʹ������1����128-127�Ͳ��ᡣ

	-Interrupt Disable(I):�жϽ�ֹ��־
		�˱�־������ϵͳ����IRQs������жϡ�����ͨ��SEIָ��ȥ����������Σ���Ϊ1��������CLIȥ�ر����Σ����Ϊ0����

	-Decimal Mode(D):
		�����־��������ʹ6502����Decimal Mode(ǰ����ܹ���)����������2A03��֧��BCDģʽ�����Ծ�����һλ�����Ա����ã����ǲ���������κ�Ӱ�졣����ͨ��SEDָ��ȥ��1������CLDָ��ȥ��0.

	-Break Command(B):
		��������BRK(Break)ָ��ձ�ִ�й���������IRQ�жϣ��ƺ���NES�в��Ǻ���Ҫ��

	-Overflow Flag(V):�����־
		���ڱ�־��������(two��s complement)��ʾ����������ʱ����������������˷Ƿ��Ľ������������������Ͳ����˸�����һ�����ӣ�64+64�����ö����Ʊ�ʾΪ10000000,���ղ������Ϊ-128.��������������ȷ�Ľ��ʱ��V��Ϊ1.
		��ô��μ����������������أ���ͨ�����bit 6��bit7�Ľ�λ�Լ�bit7��C(��λ��־)�Ľ�λ���жϵġ�

	-Negative Flag(N):������־
		bit7(���λ)��ʾ����λ���������1�Ļ�����ʾ��ǰ���Ǹ�������N��Ϊ1��
*/
#pragma once
//#include "../Emulator/Emulator.h"
#include "../ROM/MemoryHandler.h"
#include "../Types.h"
#include "Instructions.h"
#include <vector>
#include <string>

class Emulator;

const int CPU_MEMORY_SIZE = 0xFFFF + 1;

class CPU
{
	//std::vector<std::string> logs;

	Emulator* emulator;
public:
	friend class Emulator;
#pragma region Registers

private:
	//Byte registerP;		//Status Register	״̬�Ĵ���
	/*********************************
	 Status Register
	 7 6 5 4 3 2 1 0
	 N V   B D I Z C
	*********************************/

	struct REGISTER
	{
	private:
		Byte A;		//Accumulator		�����Ĵ���
		Byte X;		//Index Register	��ַ�Ĵ���
		Byte Y;		//Index Register	��ַ�Ĵ���
	public:
		const Byte CarryBit = 0b00000001;
		const Byte ZeroBit = 0b00000010;
		const Byte InterruptDisabledBit = 0b00000100;
		const Byte DecimalModeBit = 0b00001000;
		const Byte BreakSourceBit = 0b00010000;
		const Byte UnusedBit = 0b00100000;
		const Byte OverflowBit = 0b01000000;
		const Byte NegativeBit = 0b10000000;

		Byte SP;	//Stack Pointer		ջָ��Ĵ���

		Word PC;	//Program Counter	���������

		Byte getA();
		void setA(Byte val);

		Byte getX();
		void setX(Byte val);

		Byte getY();
		void setY(Byte val);

		struct
		{
		public:
			bool Carry;
			bool Zero;
			bool InterruptDisabled;
			bool DecimalMode;
			bool BreakSource;
			bool Unused;
			bool Overflow;
			bool Negative;

		}P;

		void setFlagNZ(Byte val);
		void setP(Byte val);
		Byte getP();
	};
public:

	/*״̬�Ĵ�����־λ�ǵķ�����*/

	REGISTER Register;

#pragma endregion 

#pragma region Memory

private:
	Byte memory[CPU_MEMORY_SIZE];
	bool memoryAddressHasValue;
	Word currentMemoryAddress;

	Byte rmwValue;

public:
	ROM::MemoryHandler memoryHandler;
	void ResetInstructionAddressingMode();

	Word Address();
	Byte AddressRead();
	void AddressWrite(Byte val);

	Byte ReadByte(Word address);
	void WriteByte(Word address, Byte value);
	Word ReadWord(Word address);

	// WARN: The macro here is used to slove a msvc compile error, PLEASE DO NOT change them!
#define NextByte() ReadByte(Register.PC++)
#define NextSByte() ((SByte)ReadByte(Register.PC++))
	//Byte NextByte();
	Word NextWord();
	//SByte NextSByte();

	void Push(Byte value);
	Byte Pop();

	void PushWord(Word value);
	Word PopWord();

#pragma endregion

#pragma region Instructions

private:
	/*instructions*/
	void JSR();
	void RTI();
	void RTS();
	void INY();
	void DEY();
	void INX();
	void DEX();
	void TAY();
	void TYA();
	void TAX();
	void TXA();
	void TSX();
	void TXS();
	void PHP();
	void PLP();
	void PLA();
	void PHA();
	void BIT();
	void JMP();
	void BCS();
	void BCC();
	void BEQ();
	void BNE();
	void BVS();
	void BVC();
	void BPL();
	void BMI();
	void STA();
	void STX();
	void STY();
	void CLC();
	void SEC();
	void CLI();
	void SEI();
	void CLV();
	void CLD();
	void SED();
	void NOP();
	void LDA();
	void LDY();
	void LDX();
	void ORA();
	void AND();
	void EOR();
	void SBC();
	void ADC();
	void BRK();
	void CMP();
	void CPX();
	void CPY();
	void LSR();
	void ASL();
	void ROR();
	void ROL();
	void INC();
	void DEC();
	//unofficial opcodes
	void SKB();
	void ANC();
	void ALR();
	void ARR();
	void ATX();

	void Branch(bool cond);
	void ADCImpl(Byte value);
	void CMPImpl(Byte reg);

	Word currentInstruction;

	OpcodeDefinition opcodes[256];
public:
	int cycle;

#pragma endregion

#pragma region IORegisters
	void WriteIORegister(Word reg, Byte val);
	Byte ReadIORegister(Word reg);
#pragma endregion


#pragma region Core

private:
	bool interrupts[2];
public:
	CPU(Emulator* const _emulator);

	void ExecuteSingleInstruction();

#pragma endregion

};

