#pragma once

class Emulator;

namespace Mapper
{
	class BaseMapper;
	BaseMapper* LoadMapper(Emulator* emulator, int mapperNumber);
}