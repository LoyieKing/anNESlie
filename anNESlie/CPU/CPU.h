/*
	《NES模拟器开发 - 资料归纳整理 - CPU篇》
	http://www.it165.net/embed/html/201312/2367.html

	《NES模拟器笔记（1）入门简介》
	https://www.tuicool.com/articles/vYVraq

*/

/*
	Status Register
	7 6 5 4 3 2 1 0
	N V   B D I Z C

	-Carry Flag(C)：进位标志
		当前一条指令的计算在bit7(最高位)发生了进位时会被置为1.
		例如两个16位数相加，会先计算低8位。比如这里两个数的低8位之和为256，超过了255，此时和为0（256 - 256 = 0），由于在最高位发生了进位，C置为1.于是在计算高8位时，可以把C中的1作为低8位进上来的位。也就是说，通过C，可以进行超过8位的运算，计算能力被扩展了。
		也可以通过SEC指令(置为1)和CLC指令（清除为0）两条指令去改写C。

	-Zero Flag(Z)：零标志
		如果前一条指令产生的结果为0，比如128-128就会使得它置1，而128-127就不会。

	-Interrupt Disable(I):中断禁止标志
		此标志可以让系统屏蔽IRQs发起的中断。可以通过SEI指令去开启这个屏蔽（置为1），或者CLI去关闭屏蔽（清除为0）。

	-Decimal Mode(D):
		这个标志本来可以使6502进入Decimal Mode(前面介绍过了)，不过由于2A03不支持BCD模式，所以尽管这一位还可以被设置，但是并不会产生任何影响。可以通过SED指令去置1，或者CLD指令去清0.

	-Break Command(B):
		用来表明BRK(Break)指令刚被执行过，引起了IRQ中断（似乎在NES中不是很重要）

	-Overflow Flag(V):溢出标志
		用于标志两个补码(two’s complement)表示的数在运算时产生了溢出，导致了非法的结果。比如两个正数求和产生了负数。一个例子，64+64，和用二进制表示为10000000,按照补码规则，为-128.当产生这样不正确的结果时，V置为1.
		那么如何检测这种情况发生了呢？是通过检测bit 6到bit7的进位以及bit7到C(进位标志)的进位来判断的。

	-Negative Flag(N):负数标志
		bit7(最高位)表示符号位，如果它是1的话（表示当前数是负数），N置为1。
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
	Byte registerA;		//Accumulator		算数寄存器
	Byte registerX;		//Index Register	地址寄存器
	Byte registerY;		//Index Register	地址寄存器
	Byte registerSP;	//Stack Pointer		栈指针寄存器
	Word registerPC;	//Program Counter	程序计数器
	Byte registerP;		//Status Register	状态寄存器
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

	/*状态寄存器标志位们的访问器*/
	void setFlag_N_Z(Byte val);		//设置负数标志_零标志

	bool getFlag_N();				//负数标志访问器
	void setFlag_N(bool val);

	bool getFlag_V();				//溢出标志访问器
	void setFlag_V(bool val);

	bool getFlag_Unused();			//未使用标志位
	void setFlag_Unused(bool val);

	bool getFlag_B();				//BRK标志访问器
	void setFlag_B(bool val);

	bool getFlag_D();				//BCD标志访问器
	void setFlag_D(bool val);

	bool getFlag_I();				//中断禁止标志访问器
	void setFlag_I(bool val);

	bool getFlag_Z();				//零标志访问器
	void setFlag_Z(bool val);

	bool getFlag_C();				//进位标志访问器
	void setFlag_C(bool val);


	/*寄存器们的访问器*/
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

