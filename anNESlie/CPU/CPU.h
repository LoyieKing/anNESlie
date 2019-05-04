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


class Emulator;

const int CPU_MEMORY_SIZE = 0xFFFF;

class CPU
{
	Emulator* emulator;
public:
	friend class Emulator;
#pragma region Registers

private:
	Byte registerA;		//Accumulator		�����Ĵ���
	Byte registerX;		//Index Register	��ַ�Ĵ���
	Byte registerY;		//Index Register	��ַ�Ĵ���
	Byte registerSP;	//Stack Pointer		ջָ��Ĵ���
	Word registerPC;	//Program Counter	���������
	Byte registerP;		//Status Register	״̬�Ĵ���
	/*********************************
	 Status Register
	 7 6 5 4 3 2 1 0
	 N V   B D I Z C
	*********************************/

public:
	const Byte CarryBit = 0b00000001;
	const Byte ZeroBit = 0b00000010;
	const Byte InterruptDisabledBit = 0b00000100;
	const Byte DecimalModeBit = 0b00001000;
	const Byte BreakSourceBit = 0b00010000;
	const Byte UnusedBit = 0b00100000;
	const Byte OverflowBit = 0b01000000;
	const Byte NegativeBit = 0b10000000;

	/*״̬�Ĵ�����־λ�ǵķ�����*/
	void setFlag_N_Z(Byte val);		//���ø�����־_���־

	bool getFlag_N();				//������־������
	void setFlag_N(bool val);

	bool getFlag_V();				//�����־������
	void setFlag_V(bool val);

	bool getFlag_Unused();			//δʹ�ñ�־λ
	void setFlag_Unused(bool val);

	bool getFlag_B();				//BRK��־������
	void setFlag_B(bool val);

	bool getFlag_D();				//BCD��־������
	void setFlag_D(bool val);

	bool getFlag_I();				//�жϽ�ֹ��־������
	void setFlag_I(bool val);

	bool getFlag_Z();				//���־������
	void setFlag_Z(bool val);

	bool getFlag_C();				//��λ��־������
	void setFlag_C(bool val);


	/*�Ĵ����ǵķ�����*/
	Byte getA();
	void setA(Byte val);

	Byte getX();
	void setX(Byte val);

	Byte getY();
	void setY(Byte val);

	Byte getSP();
	void setSP(Byte val);

	Word getPC();
	void setPC(Word val);

	Byte getP();
	void setP(Byte val);
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
	Byte ReadWord(Word address);
	void WriteWord(Word address, Byte value);

	Byte NextByte();
	Word NextWord();
	SByte NextSByte();

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

