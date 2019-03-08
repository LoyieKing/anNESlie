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

	class MemoryHandler
	{
	private:
		Byte* memory;
		std::function<Byte(int)> *readHandlers;
		std::function<void(int, Byte)>  *writeHandlers;

	public:
		MemoryHandler(Byte* memory_adress,int memory_size);
		~MemoryHandler();

		inline Byte* GetMemoryAdress();
		inline void SetMemoryAdress(Byte* memory_adress);

		inline void SetReadHandler(int adress, Byte(*handler)(int));
		inline void SetReadHandler(int adress, std::function<Byte(int)> handler);
		inline void SetReadHandler(int adress_start, int adress_end, std::function<Byte(int)> handler);
		inline void SetWriteHandler(int adress, void (*handler)(int, Byte));
		inline void SetWriteHandler(int adress, std::function<void(int, Byte)>  handler);
		inline void SetWriteHandler(int adress_start, int adress_end, std::function<void(int, Byte)>  handler);

		inline Byte ReadByte(int adress);
		inline void WriteByte(int adress, Byte value);
		
		short ReadWord(int adress);
		void WriteWord(int adress, short value);

	};
}



inline Byte * ROM::MemoryHandler::GetMemoryAdress()
{
	return memory;
}

inline void ROM::MemoryHandler::SetMemoryAdress(Byte * memory_adress)
{
	memory = memory_adress;
}

inline void ROM::MemoryHandler::SetReadHandler(int adress, Byte(*handler)(int))
{
	if (handler == nullptr)
		readHandlers[adress] = [this](int adress) {return memory[adress]; };
	readHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetReadHandler(int adress, std::function<Byte(int)> handler)
{
	readHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetReadHandler(int adress_start, int adress_end, std::function<Byte(int)> handler)
{
	for (int i = adress_start; i < adress_end; i++)
		SetReadHandler(i, handler);
}

inline void ROM::MemoryHandler::SetWriteHandler(int adress, void(*handler)(int, Byte))
{
	if (handler == nullptr)
		writeHandlers[adress] = [this](int adress, Byte value) {memory[adress] = value; };
	writeHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetWriteHandler(int adress, std::function<void(int, Byte)>  handler)
{
	writeHandlers[adress] = handler;
}

inline void ROM::MemoryHandler::SetWriteHandler(int adress_start, int adress_end, std::function<void(int, Byte)>  handler)
{
	for (int i = adress_start; i < adress_end; i++)
		SetWriteHandler(i, handler);
}

inline Byte ROM::MemoryHandler::ReadByte(int adress)
{
	if (readHandlers[adress] == nullptr)
		return memory[adress];
	else
		return readHandlers[adress](adress);
}

inline void ROM::MemoryHandler::WriteByte(int adress, Byte value)
{
	if (writeHandlers[adress] == nullptr)
		memory[adress] = value;
	else
		writeHandlers[adress](adress, value);
}
