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


namespace CPU
{
	class Registers
	{
	private:
		char registerA;		//Accumulator		�����Ĵ���
		char registerX;		//Index Register	��ַ�Ĵ���
		char registerY;		//Index Register	��ַ�Ĵ���
		char registerSP;	//Stack Pointer		ջָ��Ĵ���
		char registerPC;	//Program Counter	���������
		char registerP;		//Status Register	״̬�Ĵ���
		/*********************************
		 Status Register
		 7 6 5 4 3 2 1 0
		 N V   B D I Z C
		*********************************/
		
		/*״̬�Ĵ�����־λ�ǵķ�����*/
		inline void setFlag_N_V(char val);		//�������־_������־

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
		inline char getA();
		inline void setA(char val);

		inline char getX();
		inline void setX(char val);

		inline char getY();
		inline void setY(char val);
		
		inline char getSP();
		inline void setSP(char val);
		
		inline char getPC();
		inline void setPC(char val);

		inline char getP();
		inline void setP(char val);


	};


}