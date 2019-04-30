#pragma once
#include <functional>
#include "..\Types.h"
//using namespace std;

namespace ROM
{
	/*
		此类的用处：
			1.NES的内存不是简单地线性关系，存在大量内存之间的映射、Mapper，所以创建这一个类来统一处理这类问题
			2.不在此类内直接new出内存而是交给外部管理，是为了简化ROM的Mapper交换过程（本类因此被称为Handler）
	*/

	/*
	NES为典型的八位机，不仅寄存器、内存每一位的长度同样为8bit，所以应该采用Byte
	*/

	/*
	默认的读映射应当为0
	默认的写映射应当直接return，因为一些游戏会通过向未映射地址写值的方法来继续工作
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


