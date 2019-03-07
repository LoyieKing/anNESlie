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



inline char * ROM::MemoryHandler::GetMemoryAdress()
{
	return memory;
}

inline void ROM::MemoryHandler::SetMemoryAdress(char * memory_adress)
{
	memory = memory_adress;
}

inline void ROM::MemoryHandler::SetReadHandler(int adress, char(*handler)(int))
{
	if (handler == nullptr)
		readHandlers[adress] = [this](int adress) {return memory[adress]; };
	readHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetReadHandler(int adress, std::function<char(int)> handler)
{
	readHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetReadHandler(int adress_start, int adress_end, std::function<char(int)> handler)
{
	for (int i = adress_start; i < adress_end; i++)
		SetReadHandler(i, handler);
}

inline void ROM::MemoryHandler::SetWriteHandler(int adress, void(*handler)(int, char))
{
	if (handler == nullptr)
		writeHandlers[adress] = [this](int adress, char value) {memory[adress] = value; };
	writeHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetWriteHandler(int adress, std::function<void(int, char)>  handler)
{
	writeHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetWriteHandler(int adress_start, int adress_end, std::function<void(int, char)>  handler)
{
	for (int i = adress_start; i < adress_end; i++)
		SetWriteHandler(i, handler);
}

inline char ROM::MemoryHandler::ReadByte(int adress)
{
	if (readHandlers[adress] == nullptr)
		return memory[adress];
	else
		return readHandlers[adress](adress);
}

inline void ROM::MemoryHandler::WriteByte(int adress, char value)
{
	if (writeHandlers[adress] == nullptr)
		memory[adress] = value;
	else
		writeHandlers[adress](adress, value);
}
