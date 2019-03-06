#pragma once
#include <functional>
//using namespace std;

namespace ROM
{
	/*
		������ô���
			1.NES���ڴ治�Ǽ򵥵����Թ�ϵ�����ڴ����ڴ�֮���ӳ�䡢Mapper�����Դ�����һ������ͳһ������������
			2.���ڴ�����ֱ��new���ڴ���ǽ����ⲿ������Ϊ�˼�ROM��Mapper�������̣�������˱���ΪHandler��
	*/

	/*
	NESΪ���͵İ�λ���������Ĵ������ڴ�ÿһλ�ĳ���ͬ��Ϊ8bit������Ӧ�ò���char
	*/

	class MemoryHandler
	{
	private:
		char* memory;
		std::function<char(int)> *readHandlers;
		std::function<void(int, char)>  *writeHandlers;

	public:
		MemoryHandler(char* memory_adress,int memory_size);
		~MemoryHandler();

		inline char* GetMemoryAdress();
		inline void SetMemoryAdress(char* memory_adress);

		inline void SetReadHandler(int adress, char(*handler)(int));
		inline void SetReadHandler(int adress, std::function<char(int)> handler);
		inline void SetReadHandler(int adress_start, int adress_end, std::function<char(int)> handler);
		inline void SetWriteHandler(int adress, void (*handler)(int, char));
		inline void SetWriteHandler(int adress, std::function<void(int, char)>  handler);
		inline void SetWriteHandler(int adress_start, int adress_end, std::function<void(int, char)>  handler);

		inline char ReadByte(int adress);
		inline void WriteByte(int adress, char value);
		
		short ReadWord(int adress);
		void WriteWord(int adress, short value);

	};
}