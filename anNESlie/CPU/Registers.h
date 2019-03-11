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

#include"..\Types.h"

namespace CPU
{
	class Registers
	{
	private:
		Byte registerA;		//Accumulator		算数寄存器
		Byte registerX;		//Index Register	地址寄存器
		Byte registerY;		//Index Register	地址寄存器
		Byte registerSP;	//Stack Pointer		栈指针寄存器
		Byte registerPC;	//Program Counter	程序计数器
		Byte registerP;		//Status Register	状态寄存器
		/*********************************
		 Status Register
		 7 6 5 4 3 2 1 0
		 N V   B D I Z C
		*********************************/

	public:
		const Byte CarryBit = 0b00000001;
		const Byte ZeroBit = 0b00000010;
		const Byte InterruptDisabledBit	= 0b00000100;
		const Byte DecimalModeBit = 0b00001000;
		const Byte BreakSourceBit = 0b00010000;
		const Byte UnusedBit = 0b00100000;
		const Byte OverflowBit = 0b01000000;
		const Byte NegativeBit = 0b10000000;

		/*状态寄存器标志位们的访问器*/
		inline void setFlag_N_Z(Byte val);		//设置负数标志_零标志

		inline bool getFlag_N();				//负数标志访问器
		inline void setFlag_N(bool val);

		inline bool getFlag_V();				//溢出标志访问器
		inline void setFlag_V(bool val);

		inline bool getFlag_Unused();			//未使用标志位
		inline void setFlag_Unused(bool val);

		inline bool getFlag_B();				//BRK标志访问器
		inline void setFlag_B(bool val);

		inline bool getFlag_D();				//BCD标志访问器
		inline void setFlag_D(bool val);

		inline bool getFlag_I();				//中断禁止标志访问器
		inline void setFlag_I(bool val);

		inline bool getFlag_Z();				//零标志访问器
		inline void setFlag_Z(bool val);

		inline bool getFlag_C();				//进位标志访问器
		inline void setFlag_C(bool val);


		/*寄存器们的访问器*/
		inline Byte getA();
		inline void setA(Byte val);

		inline Byte getX();
		inline void setX(Byte val);

		inline Byte getY();
		inline void setY(Byte val);

		inline Byte getSP();
		inline void setSP(Byte val);

		inline Byte getPC();
		inline void setPC(Byte val);

		inline Byte getP();
		inline void setP(Byte val);


	};


}

inline void CPU::Registers::setFlag_N_Z(Byte val)
{
	if (val == 0)
		registerP |= ZeroBit;
	if (val < 0)
		registerP |= NegativeBit;
}

inline bool CPU::Registers::getFlag_N()
{
	return registerP & NegativeBit;
}

inline void CPU::Registers::setFlag_N(bool val)
{
	if (val)
		registerP |= NegativeBit;
	else
		registerP &= NegativeBit;
}

inline bool CPU::Registers::getFlag_V()
{
	return registerP & OverflowBit;
}

inline void CPU::Registers::setFlag_V(bool val)
{
	if (val)
		registerP |= OverflowBit;
	else
		registerP &= ~OverflowBit;
}

inline bool CPU::Registers::getFlag_Unused()
{
	return registerP & UnusedBit;
}

inline void CPU::Registers::setFlag_Unused(bool val)
{
	if (val)
		registerP |= UnusedBit;
	else
		registerP &= ~UnusedBit;
}

inline bool CPU::Registers::getFlag_B()
{
	return registerP & BreakSourceBit;
}

inline void CPU::Registers::setFlag_B(bool val)
{
	if (val)
		registerP |= BreakSourceBit;
	else
		registerP &= ~BreakSourceBit;
}

inline bool CPU::Registers::getFlag_D()
{
	return registerP & DecimalModeBit;
}

inline void CPU::Registers::setFlag_D(bool val)
{
	if (val)
		registerP |= DecimalModeBit;
	else
		registerP &= ~DecimalModeBit;
}

inline bool CPU::Registers::getFlag_I()
{
	return registerP & InterruptDisabledBit;
}

inline void CPU::Registers::setFlag_I(bool val)
{
	if (val)
		registerP |= InterruptDisabledBit;
	else
		registerP &= ~InterruptDisabledBit;
}

inline bool CPU::Registers::getFlag_Z()
{
	return registerP & ZeroBit;
}

inline void CPU::Registers::setFlag_Z(bool val)
{
	if (val)
		registerP |= ZeroBit;
	else
		registerP &= ~ZeroBit;
}

inline bool CPU::Registers::getFlag_C()
{
	return registerP & CarryBit;
}

inline void CPU::Registers::setFlag_C(bool val)
{
	if (val)
		registerP |= CarryBit;
	else
		registerP &= ~CarryBit;
}


inline Byte CPU::Registers::getA()
{
	return registerA;
}

inline void CPU::Registers::setA(Byte val)
{
	registerA = val;
}

inline Byte CPU::Registers::getX()
{
	return registerX;
}

inline void CPU::Registers::setX(Byte val)
{
	registerX = val;
}

inline Byte CPU::Registers::getY()
{
	return registerY;
}

inline void CPU::Registers::setY(Byte val)
{
	registerY = val;
}

inline Byte CPU::Registers::getSP()
{
	return registerSP;
}

inline void CPU::Registers::setSP(Byte val)
{
	registerSP = val;
}

inline Byte CPU::Registers::getPC()
{
	return registerPC;
}

inline void CPU::Registers::setPC(Byte val)
{
	registerPC = val;
}

inline Byte CPU::Registers::getP()
{
	return registerP;
}

inline void CPU::Registers::setP(Byte val)
{
	registerP = val;
}
