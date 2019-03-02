/*
	�������ò�Ʒϵͳ�ļ���9.NES�ļ���ʽ
	ƫ��	�ֽ���	����
	0��3	4		�ַ�����NES^Z������ʶ��.NES�ļ�
	4		1		16kB ROM����Ŀ
	5		1		8kB VROM����Ŀ
	6		1		D0��1����ֱ����0��ˮƽ����
��	��				D1��1���е�ؼ��䣬SRAM��ַ$6000-$7FFF
��	��				D2��1����$7000-$71FF��һ��512�ֽڵ�trainer
��	��				D3��1��4��ĻVRAM����
��	��				D4��D7��ROM Mapper�ĵ�4λ
	7		1		D0��D3��������������0��׼����Ϊ��Mapper��^_^��
��	��				D4��D7��ROM Mapper�ĸ�4λ
	8��F	8		������������0
	16-	1	6KxM	ROM���������У��������trainer������512�ֽڰ���ROM��֮ǰ
	-EOF	8KxN	VROM��, ��������
*/

/*
	https://www.tuicool.com/articles/vYVraq
*/

#pragma once
#include "../stdafx.h"

namespace ROM
{
	enum VRAMMirroringMode
	{
		Horizontal,
		Vertical,
		All,
		Upper,
		Lower
	};


	class Cartridge
	{
	private:
		/*ROM��������*/
		typedef struct
		{
			char* Raw;
			int PRGROMSize;
			int CHRROMSize;
			int PRGROMOffset;
			int MapperNumber;
			void* PRGROM;
			void* CHRROM;
			VRAMMirroringMode MirroringMode;

			bool hasTrainer;
			void* Trainer;
		}Data;

		Data* data;

		/*���ü���ʵ��*/
		int* ref_count;

	public:
		enum
		{
			FILE_OPEN_FAIL,
			ROM_FORMAT_WRONG,
			OUT_OF_MEMORY
		}READ_EXCEPTION;

		Cartridge(CString file_name);
		Cartridge(const Cartridge&);
		~Cartridge();


		void const* getRaw();	//��ȡ����ROM�ļ�ָ��
		int getPRGROMSize();	//Program Memory����Ϸ��������
		int getCHRROMSize();	//Character Memory��������ʾ����ͼ�ȣ����ݵĴ洢��
		int getPRGROMOffset();
		int getMapperNumber();	//Mapper����
		void const* getPRGROM();
		void const* getCHRROM();
		VRAMMirroringMode getMirroringMode();
		void* getTrainer();	//������Tranier�򷵻�nullptr
	};



}