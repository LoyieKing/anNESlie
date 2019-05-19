#include "Cartridge.h"
#include <fstream>

using namespace ROM;

Cartridge::Cartridge(const char* file_name)
{
	std::ifstream file;
	file.open(file_name, std::ios::binary);
	if (file.fail())
		throw Cartridge::FILE_OPEN_FAIL;
	/*读取文件开头四字节，判断是否为NES ROM，不是则结束，减少无意义的磁盘性能和内存浪费*/
	int ident;
	file.read((char*)(&ident), 4);
	if (ident != 0x1A53454E)// 0x1A53454E = (BYTE)"N E S 0x1A"
		throw Cartridge::ROM_FORMAT_WRONG;

	/*读入完整文件*/
	file.seekg(0, std::ios::end);
	ULONGLONG fileLength = file.tellg();
	file.seekg(0, std::ios::beg);
	

	Byte* Raw = new Byte[fileLength];
	if (Raw == nullptr)
		throw Cartridge::OUT_OF_MEMORY;

	file.read((char*)Raw, fileLength);// 排除掉开头的四个字节


	/*开始解析*/
	PRGROMSize = Raw[4] * 0x4000; // 16kb units
	CHRROMSize = Raw[5] * 0x2000; // 8kb units
	//PRGRAMSize = praw[8] * 0x2000;

	bool hasTrainer = (Raw[6] & 0b100) > 0;
	hasTrainer = hasTrainer;
	if (hasTrainer)
	{
		Trainer = Raw + 16;
		PRGROMOffset = 16 + 512;
	}
	else
	{
		Trainer = nullptr;
		PRGROMOffset = 16;
	}
	//PRGROMOffset = 16 + (hasTrainer ? 512 : 0);

	MirroringMode = (Raw[6] & 0x1) > 0 ? Vertical : Horizontal;
	if ((Raw[6] & 0x8) > 0) MirroringMode = All;

	MapperNumber = (Raw[6] >> 4) | (Raw[7] & 0xF0);

	PRGROM = Raw + PRGROMOffset;

	if (CHRROMSize == 0)
		CHRROM = new Byte[0x2000];
	else
		CHRROM = Raw + PRGROMOffset + PRGROMSize;
}