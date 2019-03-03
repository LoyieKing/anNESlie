//#include "../stdafx.h"
#include "Cartridge.h"

using namespace ROM;

Cartridge::Cartridge(CString file_name)
{
	CFile file;
	if (!file.Open(file_name, CFile::modeRead))
		throw Cartridge::FILE_OPEN_FAIL;

	/*��ȡ�ļ���ͷ���ֽڣ��ж��Ƿ�ΪNES ROM�����������������������Ĵ������ܺ��ڴ��˷�*/
	int ident;
	if (file.Read(&ident, 4) != 4)
		throw Cartridge::ROM_FORMAT_WRONG;
	if (ident != 0x1A53454E)// 0x1A53454E = (BYTE)"N E S 0x1A"
		throw Cartridge::ROM_FORMAT_WRONG;

	/*���������ļ�*/
	ULONGLONG fileLength = file.GetLength();

	data = new Data;
	char* praw = new char[fileLength];
	data->Raw = praw;
	if (praw == nullptr)
		throw Cartridge::OUT_OF_MEMORY;
	praw[0] = 'N';
	praw[1] = 'E';
	praw[2] = 'S';
	praw[3] = 0x1A;
	file.Read(praw + 4, fileLength - 4);// �ų�����ͷ���ĸ��ֽ�


	/*��ʼ����*/
	data->PRGROMSize = praw[4] * 0x4000; // 16kb units
	data->CHRROMSize = praw[5] * 0x2000; // 8kb units
	//data->PRGRAMSize = praw[8] * 0x2000;

	bool hasTrainer = (praw[6] & 0b100) > 0;
	data->hasTrainer = hasTrainer;
	if (hasTrainer)
	{
		data->Trainer = praw + 16;
		data->PRGROMOffset = 16 + 512;
	}
	else
	{
		data->Trainer = nullptr;
		data->PRGROMOffset = 16;
	}
	//data->PRGROMOffset = 16 + (hasTrainer ? 512 : 0);

	data->MirroringMode = (praw[6] & 0x1) > 0 ? Vertical : Horizontal;
	if ((praw[6] & 0x8) > 0) data->MirroringMode = All;

	data->MapperNumber = (praw[6] >> 4) | (praw[7] & 0xF0);

	data->PRGROM = praw + data->PRGROMOffset;

	if (data->CHRROMSize == 0)
		data->CHRROM = new char[0x2000];
	else
		data->CHRROM = praw + data->PRGROMOffset + data->PRGROMSize;


	/*�������ü���*/
	ref_count = new int;
	*ref_count = 1;
}

Cartridge::Cartridge(const Cartridge &rom_data)
{
	data = rom_data.data;
	ref_count = rom_data.ref_count;
	*ref_count++;
}

Cartridge::~Cartridge()
{
	*ref_count--;
	if (*ref_count == 0)
	{
		delete[] data->Raw;
		if (data->CHRROMSize == 0)
			delete[] data->CHRROM;
		delete data;

		delete ref_count;
	}
}

inline void const * ROM::Cartridge::getRaw()
{
	return data->Raw;
}

inline int ROM::Cartridge::getPRGROMSize()
{
	return data->PRGROMSize;
}

inline int ROM::Cartridge::getCHRROMSize()
{
	return data->CHRROMSize;
}

inline int ROM::Cartridge::getPRGROMOffset()
{
	return data->PRGROMOffset;
}

inline int ROM::Cartridge::getMapperNumber()
{
	return data->MapperNumber;
}

inline void const * ROM::Cartridge::getPRGROM()
{
	return data->PRGROM;
}

inline void const * ROM::Cartridge::getCHRROM()
{
	return data->CHRROM;
}

inline VRAMMirroringMode ROM::Cartridge::getMirroringMode()
{
	return data->MirroringMode;
}

inline void * ROM::Cartridge::getTrainer()
{
	return data->hasTrainer ? data->Trainer : nullptr;
}
