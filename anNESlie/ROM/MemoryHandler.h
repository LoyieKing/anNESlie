#pragma once
#include <functional>
//using namespace std;

namespace ROM
{
	/*
		此类的用处：
			1.NES的内存不是简单地线性关系，存在大量内存之间的映射、Mapper，所以创建这一个类来统一处理这类问题
			2.不在此类内直接new出内存而是交给外部管理，是为了简化ROM的Mapper交换过程（本类因此被称为Handler）
	*/

	/*
	NES为典型的八位机，不仅寄存器、内存每一位的长度同样为8bit，所以应该采用char
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