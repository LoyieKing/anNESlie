#pragma once
#include <functional>
#include "..\Types.h"
#include <vector>
#include <string>
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
		std::function<Byte(Word)>* readHandlers;
		std::function<void(Word, Byte)>* writeHandlers;

	public:
		MemoryHandler(Byte* memory_address, int memory_size);
		~MemoryHandler();


		void SetReadHandler(Word address_start, Word address_end, std::function<Byte(Word)> handler);
		void SetWriteHandler(Word address_start, Word address_end, std::function<void(Word, Byte)>  handler);

		Byte ReadByte(Word address);
		void WriteByte(Word address, Byte value);

		Word ReadWord(Word address);
		//void WriteWord(Word address, Word value);

	};
}


