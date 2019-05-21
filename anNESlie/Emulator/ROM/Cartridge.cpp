#include "Cartridge.h"
#include <fstream>

using namespace ROM;

Cartridge::Cartridge(const char* file_name)
{
	std::ifstream file;
	file.open(file_name, std::ios::binary);
	if (file.fail())
		throw Cartridge::FILE_OPEN_FAIL;
	/*��ȡ�ļ���ͷ���ֽڣ��ж��Ƿ�ΪNES ROM�����������������������Ĵ������ܺ��ڴ��˷�*/
	int ident;
	file.read((char*)(&ident), 4);
	if (ident != 0x1A53454E)// 0x1A53454E = (BYTE)"N E S 0x1A"
		throw Cartridge::ROM_FORMAT_WRONG;

	/*���������ļ�*/
	file.seekg(0, std::ios::end);
	ULONGLONG fileLength = file.tellg();
	file.seekg(0, std::ios::beg);
	

	Byte* Raw = new Byte[fileLength];
	if (Raw == nullptr)
		throw Cartridge::OUT_OF_MEMORY;

	file.read((char*)Raw, fileLength);// �ų�����ͷ���ĸ��ֽ�


	/*��ʼ����*/
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