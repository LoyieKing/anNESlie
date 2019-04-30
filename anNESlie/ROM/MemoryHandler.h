#pragma once
#include <functional>
#include "..\Types.h"
//using namespace std;

namespace ROM
{
	/*
		������ô���
			1.NES���ڴ治�Ǽ򵥵����Թ�ϵ�����ڴ����ڴ�֮���ӳ�䡢Mapper�����Դ�����һ������ͳһ������������
			2.���ڴ�����ֱ��new���ڴ���ǽ����ⲿ������Ϊ�˼�ROM��Mapper�������̣�������˱���ΪHandler��
	*/

	/*
	NESΪ���͵İ�λ���������Ĵ������ڴ�ÿһλ�ĳ���ͬ��Ϊ8bit������Ӧ�ò���Byte
	*/

	/*
	Ĭ�ϵĶ�ӳ��Ӧ��Ϊ0
	Ĭ�ϵ�дӳ��Ӧ��ֱ��return����ΪһЩ��Ϸ��ͨ����δӳ���ַдֵ�ķ�������������
	*/

	class MemoryHandler
	{
	private:
		Byte* memory;
		std::function<Byte(int)>* readHandlers;
		std::function<void(int, Byte)>* writeHandlers;

	public:
		MemoryHandler(Byte* memory_adress, int memory_size);
		~MemoryHandler();

		Byte* GetMemoryAdress();
		void SetMemoryAdress(Byte* memory_adress);

		void SetReadHandler(Word adress, Byte(*handler)(int));
		void SetReadHandler(Word adress, std::function<Byte(int)> handler);
		void SetReadHandler(Word adress_start, int adress_end, std::function<Byte(int)> handler);
		void SetWriteHandler(Word adress, void (*handler)(int, Byte));
		void SetWriteHandler(Word adress, std::function<void(int, Byte)>  handler);
		void SetWriteHandler(Word adress_start, int adress_end, std::function<void(int, Byte)>  handler);

		Byte ReadByte(Word adress);
		void WriteByte(Word adress, Byte value);

		Byte ReadWord(Word adress);
		void WriteWord(Word adress, Word value);

	};
}


