#pragma once


namespace ROM
{
	/*
		NESΪ���͵İ�λ���������Ĵ������ڴ�ÿһλ�ĳ���ͬ��Ϊ8bit������Ӧ�ò���char
	*/

	typedef char(*ReadHandler)(int adress);
	typedef void(*WriteHandler)(int adress, char value);

	class Memory
	{
	private:
		char* memory;
		ReadHandler *readHandlers;
		WriteHandler *writeHandlers;

	public:
		Memory(int memory_size);
		~Memory();

		void SetReadHandler(int adress, ReadHandler handler);
		void SetReadHandler(int adress_start, int adress_end, ReadHandler handler);
		void SetWriteHandler(int adress, WriteHandler handler);
		void SetWriteHandler(int adress_start, int adress_end, WriteHandler handler);

		char ReadByte(int adress);
		void WriteByte(int adress, char value);
		
		short ReadWord(int adress);
		void WriteWord(int adress, short value);

	};
}