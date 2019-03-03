/*
	《任天堂产品系统文件》9.NES文件格式
	偏移	字节数	内容
	0－3	4		字符串“NES^Z”用来识别.NES文件
	4		1		16kB ROM的数目
	5		1		8kB VROM的数目
	6		1		D0：1＝垂直镜像，0＝水平镜像
　	　				D1：1＝有电池记忆，SRAM地址$6000-$7FFF
　	　				D2：1＝在$7000-$71FF有一个512字节的trainer
　	　				D3：1＝4屏幕VRAM布局
　	　				D4－D7：ROM Mapper的低4位
	7		1		D0－D3：保留，必须是0（准备作为副Mapper号^_^）
　	　				D4－D7：ROM Mapper的高4位
	8－F	8		保留，必须是0
	16-	1	6KxM	ROM段升序排列，如果存在trainer，它的512字节摆在ROM段之前
	-EOF	8KxN	VROM段, 升序排列
*/

/*
	《NES模拟器笔记（1）入门简介》
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
		/*ROM数据区域*/
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

		/*引用计数实现*/
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


		inline void const* getRaw();	//获取完整ROM文件指针
		inline int getPRGROMSize();	//Program Memory，游戏程序数据
		inline int getCHRROMSize();	//Character Memory，用于显示（贴图等）数据的存储。
		inline int getPRGROMOffset();
		inline int getMapperNumber();	//Mapper数量
		inline void const* getPRGROM();
		inline void const* getCHRROM();
		inline VRAMMirroringMode getMirroringMode();
		inline void* getTrainer();	//不存在Tranier则返回nullptr
	};



}