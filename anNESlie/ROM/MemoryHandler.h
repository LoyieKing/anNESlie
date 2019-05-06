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
		bool log;
		std::vector<std::string> logs;

		Byte* memory;
		std::function<Byte(int)>* readHandlers;
		std::function<void(int, Byte)>* writeHandlers;

	public:
		MemoryHandler(Byte* memory_address, int memory_size, bool log = false);
		~MemoryHandler();

		Byte* GetMemoryAddress();
		void SetMemoryAddress(Byte* memory_address);

		void SetReadHandler(Word address, Byte(*handler)(int));
		void SetReadHandler(Word address, std::function<Byte(int)> handler);
		void SetReadHandler(Word address_start, int address_end, std::function<Byte(int)> handler);
		void SetWriteHandler(Word address, void (*handler)(int, Byte));
		void SetWriteHandler(Word address, std::function<void(int, Byte)>  handler);
		void SetWriteHandler(Word address_start, int address_end, std::function<void(int, Byte)>  handler);

		Byte ReadByte(Word address);
		void WriteByte(Word address, Byte value);

		Word ReadWord(Word address);
		//void WriteWord(Word address, Word value);

	};
}


