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

#include"..\Types.h"

namespace CPU
{
	class Registers
	{
	private:
		Byte registerA;		//Accumulator		�����Ĵ���
		Byte registerX;		//Index Register	��ַ�Ĵ���
		Byte registerY;		//Index Register	��ַ�Ĵ���
		Byte registerSP;	//Stack Pointer		ջָ��Ĵ���
		Byte registerPC;	//Program Counter	���������
		Byte registerP;		//Status Register	״̬�Ĵ���
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

		/*״̬�Ĵ�����־λ�ǵķ�����*/
		inline void setFlag_N_Z(Byte val);		//���ø�����־_���־

		inline bool getFlag_N();				//������־������
		inline void setFlag_N(bool val);

		inline bool getFlag_V();				//�����־������
		inline void setFlag_V(bool val);

		inline bool getFlag_Unused();			//δʹ�ñ�־λ
		inline void setFlag_Unused(bool val);

		inline bool getFlag_B();				//BRK��־������
		inline void setFlag_B(bool val);

		inline bool getFlag_D();				//BCD��־������
		inline void setFlag_D(bool val);

		inline bool getFlag_I();				//�жϽ�ֹ��־������
		inline void setFlag_I(bool val);

		inline bool getFlag_Z();				//���־������
		inline void setFlag_Z(bool val);

		inline bool getFlag_C();				//��λ��־������
		inline void setFlag_C(bool val);


		/*�Ĵ����ǵķ�����*/
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
