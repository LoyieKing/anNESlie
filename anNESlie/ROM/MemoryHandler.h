#pragma once


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
	typedef char(*ReadHandler)(int adress);
	typedef void(*WriteHandler)(int adress, char value);


	class MemoryHandler
	{
	private:
		char* memory;
		ReadHandler *readHandlers;
		WriteHandler *writeHandlers;

	public:
		MemoryHandler(char* memory_adress,int memory_size);
		~MemoryHandler();

		inline char* GetMemoryAdress();
		inline void SetMemoryAdress(char* memory_adress);

		inline void SetReadHandler(int adress, ReadHandler handler);
		inline void SetReadHandler(int adress_start, int adress_end, ReadHandler handler);
		inline void SetWriteHandler(int adress, WriteHandler handler);
		inline void SetWriteHandler(int adress_start, int adress_end, WriteHandler handler);

		inline char ReadByte(int adress);
		inline void WriteByte(int adress, char value);
		
		short ReadWord(int adress);
		void WriteWord(int adress, short value);

	};
}