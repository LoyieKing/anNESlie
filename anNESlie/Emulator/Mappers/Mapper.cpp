#include "Mapper.h"

#include "BaseMapper.h"
#include "NROM.h"

Mapper::BaseMapper* Mapper::LoadMapper(Emulator* emulator, int mapperNumber)
{
	Mapper::BaseMapper* mapper;
	switch (mapperNumber)
	{
	case 0:
		mapper = new Mapper::NROM(emulator);
		break;
	default:
		mapper = nullptr;
		//throw "unsupported mapper!";
	}
	return mapper;
}
