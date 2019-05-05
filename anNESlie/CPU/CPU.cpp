#include "CPU.h"

#include "../PPU/PPU.h"
#include "../Mappers/BaseMapper.h"

#include <stdio.h>

Word InterruptHandlerOffsets[] = {
	0xFFFA, 0xFFFE, 0xFFFC
};

CPU::CPU(Emulator* const _emulator) :
	memoryAddressHasValue(false),
	// Init memory handlers
	memoryHandler(memory, CPU_MEMORY_SIZE)
{
	//file = fopen("log.txt", "w");
	interrupts[0] = 0;
	interrupts[1] = 0;
	this->cycle = 0;

	emulator = _emulator;


#pragma region OpcodesBinding
	opcodes[0x00].Opcode = 0x00;
	opcodes[0x00].Cycles = 7;
	opcodes[0x00].PageBoundary = false;
	opcodes[0x00].RMW = false;
	opcodes[0x00].Mode = None;
	opcodes[0x00].action = std::bind(&CPU::BRK, this);

	opcodes[0x01].Opcode = 0x01;
	opcodes[0x01].Cycles = 6;
	opcodes[0x01].PageBoundary = false;
	opcodes[0x01].RMW = false;
	opcodes[0x01].Mode = IndirectX;
	opcodes[0x01].action = std::bind(&CPU::ORA, this);

	opcodes[0x02].Opcode = -1;

	opcodes[0x03].Opcode = -1;

	opcodes[0x04].Opcode = -1;

	opcodes[0x05].Opcode = 0x05;
	opcodes[0x05].Cycles = 3;
	opcodes[0x05].PageBoundary = false;
	opcodes[0x05].RMW = false;
	opcodes[0x05].Mode = ZeroPage;
	opcodes[0x05].action = std::bind(&CPU::ORA, this);

	opcodes[0x06].Opcode = 0x06;
	opcodes[0x06].Cycles = 5;
	opcodes[0x06].PageBoundary = false;
	opcodes[0x06].RMW = true;
	opcodes[0x06].Mode = ZeroPage;
	opcodes[0x06].action = std::bind(&CPU::ASL, this);

	opcodes[0x07].Opcode = -1;

	opcodes[0x08].Opcode = 0x08;
	opcodes[0x08].Cycles = 3;
	opcodes[0x08].PageBoundary = false;
	opcodes[0x08].RMW = false;
	opcodes[0x08].Mode = None;
	opcodes[0x08].action = std::bind(&CPU::PHP, this);

	opcodes[0x09].Opcode = 0x09;
	opcodes[0x09].Cycles = 2;
	opcodes[0x09].PageBoundary = false;
	opcodes[0x09].RMW = false;
	opcodes[0x09].Mode = Immediate;
	opcodes[0x09].action = std::bind(&CPU::ORA, this);

	opcodes[0x0A].Opcode = 0x0A;
	opcodes[0x0A].Cycles = 2;
	opcodes[0x0A].PageBoundary = false;
	opcodes[0x0A].RMW = true;
	opcodes[0x0A].Mode = Direct;
	opcodes[0x0A].action = std::bind(&CPU::ASL, this);

	opcodes[0x0B].Opcode = 0x0B;
	opcodes[0x0B].Cycles = 2;
	opcodes[0x0B].PageBoundary = false;
	opcodes[0x0B].RMW = false;
	opcodes[0x0B].Mode = Immediate;
	opcodes[0x0B].action = std::bind(&CPU::ANC, this);

	opcodes[0x0C].Opcode = -1;

	opcodes[0x0D].Opcode = 0x0D;
	opcodes[0x0D].Cycles = 4;
	opcodes[0x0D].PageBoundary = false;
	opcodes[0x0D].RMW = false;
	opcodes[0x0D].Mode = Absolute;
	opcodes[0x0D].action = std::bind(&CPU::ORA, this);

	opcodes[0x0E].Opcode = 0x0E;
	opcodes[0x0E].Cycles = 6;
	opcodes[0x0E].PageBoundary = false;
	opcodes[0x0E].RMW = true;
	opcodes[0x0E].Mode = Absolute;
	opcodes[0x0E].action = std::bind(&CPU::ASL, this);

	opcodes[0x0F].Opcode = -1;

	opcodes[0x10].Opcode = 0x10;
	opcodes[0x10].Cycles = 2;
	opcodes[0x10].PageBoundary = false;
	opcodes[0x10].RMW = false;
	opcodes[0x10].Mode = None;
	opcodes[0x10].action = std::bind(&CPU::BPL, this);

	opcodes[0x11].Opcode = 0x11;
	opcodes[0x11].Cycles = 5;
	opcodes[0x11].PageBoundary = true;
	opcodes[0x11].RMW = false;
	opcodes[0x11].Mode = IndirectY;
	opcodes[0x11].action = std::bind(&CPU::ORA, this);

	opcodes[0x12].Opcode = -1;

	opcodes[0x13].Opcode = -1;

	opcodes[0x14].Opcode = -1;

	opcodes[0x15].Opcode = 0x15;
	opcodes[0x15].Cycles = 4;
	opcodes[0x15].PageBoundary = false;
	opcodes[0x15].RMW = false;
	opcodes[0x15].Mode = ZeroPageX;
	opcodes[0x15].action = std::bind(&CPU::ORA, this);

	opcodes[0x16].Opcode = 0x16;
	opcodes[0x16].Cycles = 6;
	opcodes[0x16].PageBoundary = false;
	opcodes[0x16].RMW = true;
	opcodes[0x16].Mode = ZeroPageX;
	opcodes[0x16].action = std::bind(&CPU::ASL, this);

	opcodes[0x17].Opcode = -1;

	opcodes[0x18].Opcode = 0x18;
	opcodes[0x18].Cycles = 2;
	opcodes[0x18].PageBoundary = false;
	opcodes[0x18].RMW = false;
	opcodes[0x18].Mode = None;
	opcodes[0x18].action = std::bind(&CPU::CLC, this);

	opcodes[0x19].Opcode = 0x19;
	opcodes[0x19].Cycles = 4;
	opcodes[0x19].PageBoundary = true;
	opcodes[0x19].RMW = false;
	opcodes[0x19].Mode = AbsoluteY;
	opcodes[0x19].action = std::bind(&CPU::ORA, this);

	opcodes[0x1A].Opcode = 0x1A;
	opcodes[0x1A].Cycles = 2;
	opcodes[0x1A].PageBoundary = false;
	opcodes[0x1A].RMW = false;
	opcodes[0x1A].Mode = None;
	opcodes[0x1A].action = std::bind(&CPU::NOP, this);

	opcodes[0x1B].Opcode = -1;

	opcodes[0x1C].Opcode = -1;

	opcodes[0x1D].Opcode = 0x1D;
	opcodes[0x1D].Cycles = 4;
	opcodes[0x1D].PageBoundary = true;
	opcodes[0x1D].RMW = false;
	opcodes[0x1D].Mode = AbsoluteX;
	opcodes[0x1D].action = std::bind(&CPU::ORA, this);

	opcodes[0x1E].Opcode = 0x1E;
	opcodes[0x1E].Cycles = 7;
	opcodes[0x1E].PageBoundary = false;
	opcodes[0x1E].RMW = true;
	opcodes[0x1E].Mode = AbsoluteX;
	opcodes[0x1E].action = std::bind(&CPU::ASL, this);

	opcodes[0x1F].Opcode = -1;

	opcodes[0x20].Opcode = 0x20;
	opcodes[0x20].Cycles = 6;
	opcodes[0x20].PageBoundary = false;
	opcodes[0x20].RMW = false;
	opcodes[0x20].Mode = None;
	opcodes[0x20].action = std::bind(&CPU::JSR, this);

	opcodes[0x21].Opcode = 0x21;
	opcodes[0x21].Cycles = 6;
	opcodes[0x21].PageBoundary = false;
	opcodes[0x21].RMW = false;
	opcodes[0x21].Mode = IndirectX;
	opcodes[0x21].action = std::bind(&CPU::AND, this);

	opcodes[0x22].Opcode = -1;

	opcodes[0x23].Opcode = -1;

	opcodes[0x24].Opcode = 0x24;
	opcodes[0x24].Cycles = 3;
	opcodes[0x24].PageBoundary = false;
	opcodes[0x24].RMW = false;
	opcodes[0x24].Mode = ZeroPage;
	opcodes[0x24].action = std::bind(&CPU::BIT, this);

	opcodes[0x25].Opcode = 0x25;
	opcodes[0x25].Cycles = 3;
	opcodes[0x25].PageBoundary = false;
	opcodes[0x25].RMW = false;
	opcodes[0x25].Mode = ZeroPage;
	opcodes[0x25].action = std::bind(&CPU::AND, this);

	opcodes[0x26].Opcode = 0x26;
	opcodes[0x26].Cycles = 5;
	opcodes[0x26].PageBoundary = false;
	opcodes[0x26].RMW = true;
	opcodes[0x26].Mode = ZeroPage;
	opcodes[0x26].action = std::bind(&CPU::ROL, this);

	opcodes[0x27].Opcode = -1;

	opcodes[0x28].Opcode = 0x28;
	opcodes[0x28].Cycles = 4;
	opcodes[0x28].PageBoundary = false;
	opcodes[0x28].RMW = false;
	opcodes[0x28].Mode = None;
	opcodes[0x28].action = std::bind(&CPU::PLP, this);

	opcodes[0x29].Opcode = 0x29;
	opcodes[0x29].Cycles = 2;
	opcodes[0x29].PageBoundary = false;
	opcodes[0x29].RMW = false;
	opcodes[0x29].Mode = Immediate;
	opcodes[0x29].action = std::bind(&CPU::AND, this);

	opcodes[0x2A].Opcode = 0x2A;
	opcodes[0x2A].Cycles = 2;
	opcodes[0x2A].PageBoundary = false;
	opcodes[0x2A].RMW = true;
	opcodes[0x2A].Mode = Direct;
	opcodes[0x2A].action = std::bind(&CPU::ROL, this);

	opcodes[0x2B].Opcode = 0x2B;
	opcodes[0x2B].Cycles = 2;
	opcodes[0x2B].PageBoundary = false;
	opcodes[0x2B].RMW = false;
	opcodes[0x2B].Mode = Immediate;
	opcodes[0x2B].action = std::bind(&CPU::ANC, this);

	opcodes[0x2C].Opcode = 0x2C;
	opcodes[0x2C].Cycles = 4;
	opcodes[0x2C].PageBoundary = false;
	opcodes[0x2C].RMW = false;
	opcodes[0x2C].Mode = Absolute;
	opcodes[0x2C].action = std::bind(&CPU::BIT, this);

	opcodes[0x2D].Opcode = 0x2D;
	opcodes[0x2D].Cycles = 4;
	opcodes[0x2D].PageBoundary = false;
	opcodes[0x2D].RMW = false;
	opcodes[0x2D].Mode = Absolute;
	opcodes[0x2D].action = std::bind(&CPU::AND, this);

	opcodes[0x2E].Opcode = 0x2E;
	opcodes[0x2E].Cycles = 6;
	opcodes[0x2E].PageBoundary = false;
	opcodes[0x2E].RMW = true;
	opcodes[0x2E].Mode = Absolute;
	opcodes[0x2E].action = std::bind(&CPU::ROL, this);

	opcodes[0x2F].Opcode = -1;

	opcodes[0x30].Opcode = 0x30;
	opcodes[0x30].Cycles = 2;
	opcodes[0x30].PageBoundary = false;
	opcodes[0x30].RMW = false;
	opcodes[0x30].Mode = None;
	opcodes[0x30].action = std::bind(&CPU::BMI, this);

	opcodes[0x31].Opcode = 0x31;
	opcodes[0x31].Cycles = 5;
	opcodes[0x31].PageBoundary = true;
	opcodes[0x31].RMW = false;
	opcodes[0x31].Mode = IndirectY;
	opcodes[0x31].action = std::bind(&CPU::AND, this);

	opcodes[0x32].Opcode = -1;

	opcodes[0x33].Opcode = -1;

	opcodes[0x34].Opcode = -1;

	opcodes[0x35].Opcode = 0x35;
	opcodes[0x35].Cycles = 4;
	opcodes[0x35].PageBoundary = false;
	opcodes[0x35].RMW = false;
	opcodes[0x35].Mode = ZeroPageX;
	opcodes[0x35].action = std::bind(&CPU::AND, this);

	opcodes[0x36].Opcode = 0x36;
	opcodes[0x36].Cycles = 6;
	opcodes[0x36].PageBoundary = false;
	opcodes[0x36].RMW = true;
	opcodes[0x36].Mode = ZeroPageX;
	opcodes[0x36].action = std::bind(&CPU::ROL, this);

	opcodes[0x37].Opcode = -1;

	opcodes[0x38].Opcode = 0x38;
	opcodes[0x38].Cycles = 2;
	opcodes[0x38].PageBoundary = false;
	opcodes[0x38].RMW = false;
	opcodes[0x38].Mode = None;
	opcodes[0x38].action = std::bind(&CPU::SEC, this);

	opcodes[0x39].Opcode = 0x39;
	opcodes[0x39].Cycles = 4;
	opcodes[0x39].PageBoundary = true;
	opcodes[0x39].RMW = false;
	opcodes[0x39].Mode = AbsoluteY;
	opcodes[0x39].action = std::bind(&CPU::AND, this);

	opcodes[0x3A].Opcode = 0x3A;
	opcodes[0x3A].Cycles = 2;
	opcodes[0x3A].PageBoundary = false;
	opcodes[0x3A].RMW = false;
	opcodes[0x3A].Mode = None;
	opcodes[0x3A].action = std::bind(&CPU::NOP, this);

	opcodes[0x3B].Opcode = -1;

	opcodes[0x3C].Opcode = -1;

	opcodes[0x3D].Opcode = 0x3D;
	opcodes[0x3D].Cycles = 4;
	opcodes[0x3D].PageBoundary = true;
	opcodes[0x3D].RMW = false;
	opcodes[0x3D].Mode = AbsoluteX;
	opcodes[0x3D].action = std::bind(&CPU::AND, this);

	opcodes[0x3E].Opcode = 0x3E;
	opcodes[0x3E].Cycles = 7;
	opcodes[0x3E].PageBoundary = false;
	opcodes[0x3E].RMW = true;
	opcodes[0x3E].Mode = AbsoluteX;
	opcodes[0x3E].action = std::bind(&CPU::ROL, this);

	opcodes[0x3F].Opcode = -1;

	opcodes[0x40].Opcode = 0x40;
	opcodes[0x40].Cycles = 6;
	opcodes[0x40].PageBoundary = false;
	opcodes[0x40].RMW = false;
	opcodes[0x40].Mode = None;
	opcodes[0x40].action = std::bind(&CPU::RTI, this);

	opcodes[0x41].Opcode = 0x41;
	opcodes[0x41].Cycles = 6;
	opcodes[0x41].PageBoundary = false;
	opcodes[0x41].RMW = false;
	opcodes[0x41].Mode = IndirectX;
	opcodes[0x41].action = std::bind(&CPU::EOR, this);

	opcodes[0x42].Opcode = -1;

	opcodes[0x43].Opcode = -1;

	opcodes[0x44].Opcode = -1;

	opcodes[0x45].Opcode = 0x45;
	opcodes[0x45].Cycles = 3;
	opcodes[0x45].PageBoundary = false;
	opcodes[0x45].RMW = false;
	opcodes[0x45].Mode = ZeroPage;
	opcodes[0x45].action = std::bind(&CPU::EOR, this);

	opcodes[0x46].Opcode = 0x46;
	opcodes[0x46].Cycles = 5;
	opcodes[0x46].PageBoundary = false;
	opcodes[0x46].RMW = true;
	opcodes[0x46].Mode = ZeroPage;
	opcodes[0x46].action = std::bind(&CPU::LSR, this);

	opcodes[0x47].Opcode = -1;

	opcodes[0x48].Opcode = 0x48;
	opcodes[0x48].Cycles = 3;
	opcodes[0x48].PageBoundary = false;
	opcodes[0x48].RMW = false;
	opcodes[0x48].Mode = None;
	opcodes[0x48].action = std::bind(&CPU::PHA, this);

	opcodes[0x49].Opcode = 0x49;
	opcodes[0x49].Cycles = 2;
	opcodes[0x49].PageBoundary = false;
	opcodes[0x49].RMW = false;
	opcodes[0x49].Mode = Immediate;
	opcodes[0x49].action = std::bind(&CPU::EOR, this);

	opcodes[0x4A].Opcode = 0x4A;
	opcodes[0x4A].Cycles = 2;
	opcodes[0x4A].PageBoundary = false;
	opcodes[0x4A].RMW = true;
	opcodes[0x4A].Mode = Direct;
	opcodes[0x4A].action = std::bind(&CPU::LSR, this);

	opcodes[0x4B].Opcode = 0x4B;
	opcodes[0x4B].Cycles = 2;
	opcodes[0x4B].PageBoundary = false;
	opcodes[0x4B].RMW = false;
	opcodes[0x4B].Mode = Immediate;
	opcodes[0x4B].action = std::bind(&CPU::ALR, this);

	opcodes[0x4C].Opcode = 0x4C;
	opcodes[0x4C].Cycles = 3;
	opcodes[0x4C].PageBoundary = false;
	opcodes[0x4C].RMW = false;
	opcodes[0x4C].Mode = None;
	opcodes[0x4C].action = std::bind(&CPU::JMP, this);

	opcodes[0x4D].Opcode = 0x4D;
	opcodes[0x4D].Cycles = 4;
	opcodes[0x4D].PageBoundary = false;
	opcodes[0x4D].RMW = false;
	opcodes[0x4D].Mode = Absolute;
	opcodes[0x4D].action = std::bind(&CPU::EOR, this);

	opcodes[0x4E].Opcode = 0x4E;
	opcodes[0x4E].Cycles = 6;
	opcodes[0x4E].PageBoundary = false;
	opcodes[0x4E].RMW = true;
	opcodes[0x4E].Mode = Absolute;
	opcodes[0x4E].action = std::bind(&CPU::LSR, this);

	opcodes[0x4F].Opcode = -1;

	opcodes[0x50].Opcode = 0x50;
	opcodes[0x50].Cycles = 2;
	opcodes[0x50].PageBoundary = false;
	opcodes[0x50].RMW = false;
	opcodes[0x50].Mode = None;
	opcodes[0x50].action = std::bind(&CPU::BVC, this);

	opcodes[0x51].Opcode = 0x51;
	opcodes[0x51].Cycles = 5;
	opcodes[0x51].PageBoundary = true;
	opcodes[0x51].RMW = false;
	opcodes[0x51].Mode = IndirectY;
	opcodes[0x51].action = std::bind(&CPU::EOR, this);

	opcodes[0x52].Opcode = -1;

	opcodes[0x53].Opcode = -1;

	opcodes[0x54].Opcode = -1;

	opcodes[0x55].Opcode = 0x55;
	opcodes[0x55].Cycles = 4;
	opcodes[0x55].PageBoundary = false;
	opcodes[0x55].RMW = false;
	opcodes[0x55].Mode = ZeroPageX;
	opcodes[0x55].action = std::bind(&CPU::EOR, this);

	opcodes[0x56].Opcode = 0x56;
	opcodes[0x56].Cycles = 6;
	opcodes[0x56].PageBoundary = false;
	opcodes[0x56].RMW = true;
	opcodes[0x56].Mode = ZeroPageX;
	opcodes[0x56].action = std::bind(&CPU::LSR, this);

	opcodes[0x57].Opcode = -1;

	opcodes[0x58].Opcode = 0x58;
	opcodes[0x58].Cycles = 2;
	opcodes[0x58].PageBoundary = false;
	opcodes[0x58].RMW = false;
	opcodes[0x58].Mode = None;
	opcodes[0x58].action = std::bind(&CPU::CLI, this);

	opcodes[0x59].Opcode = 0x59;
	opcodes[0x59].Cycles = 4;
	opcodes[0x59].PageBoundary = true;
	opcodes[0x59].RMW = false;
	opcodes[0x59].Mode = AbsoluteY;
	opcodes[0x59].action = std::bind(&CPU::EOR, this);

	opcodes[0x5A].Opcode = 0x5A;
	opcodes[0x5A].Cycles = 2;
	opcodes[0x5A].PageBoundary = false;
	opcodes[0x5A].RMW = false;
	opcodes[0x5A].Mode = None;
	opcodes[0x5A].action = std::bind(&CPU::NOP, this);

	opcodes[0x5B].Opcode = -1;

	opcodes[0x5C].Opcode = -1;

	opcodes[0x5D].Opcode = 0x5D;
	opcodes[0x5D].Cycles = 4;
	opcodes[0x5D].PageBoundary = true;
	opcodes[0x5D].RMW = false;
	opcodes[0x5D].Mode = AbsoluteX;
	opcodes[0x5D].action = std::bind(&CPU::EOR, this);

	opcodes[0x5E].Opcode = 0x5E;
	opcodes[0x5E].Cycles = 7;
	opcodes[0x5E].PageBoundary = false;
	opcodes[0x5E].RMW = true;
	opcodes[0x5E].Mode = AbsoluteX;
	opcodes[0x5E].action = std::bind(&CPU::LSR, this);

	opcodes[0x5F].Opcode = -1;

	opcodes[0x60].Opcode = 0x60;
	opcodes[0x60].Cycles = 6;
	opcodes[0x60].PageBoundary = false;
	opcodes[0x60].RMW = false;
	opcodes[0x60].Mode = None;
	opcodes[0x60].action = std::bind(&CPU::RTS, this);

	opcodes[0x61].Opcode = 0x61;
	opcodes[0x61].Cycles = 6;
	opcodes[0x61].PageBoundary = false;
	opcodes[0x61].RMW = false;
	opcodes[0x61].Mode = IndirectX;
	opcodes[0x61].action = std::bind(&CPU::ADC, this);

	opcodes[0x62].Opcode = -1;

	opcodes[0x63].Opcode = -1;

	opcodes[0x64].Opcode = -1;

	opcodes[0x65].Opcode = 0x65;
	opcodes[0x65].Cycles = 3;
	opcodes[0x65].PageBoundary = false;
	opcodes[0x65].RMW = false;
	opcodes[0x65].Mode = ZeroPage;
	opcodes[0x65].action = std::bind(&CPU::ADC, this);

	opcodes[0x66].Opcode = 0x66;
	opcodes[0x66].Cycles = 5;
	opcodes[0x66].PageBoundary = false;
	opcodes[0x66].RMW = true;
	opcodes[0x66].Mode = ZeroPage;
	opcodes[0x66].action = std::bind(&CPU::ROR, this);

	opcodes[0x67].Opcode = -1;

	opcodes[0x68].Opcode = 0x68;
	opcodes[0x68].Cycles = 4;
	opcodes[0x68].PageBoundary = false;
	opcodes[0x68].RMW = false;
	opcodes[0x68].Mode = None;
	opcodes[0x68].action = std::bind(&CPU::PLA, this);

	opcodes[0x69].Opcode = 0x69;
	opcodes[0x69].Cycles = 2;
	opcodes[0x69].PageBoundary = false;
	opcodes[0x69].RMW = false;
	opcodes[0x69].Mode = Immediate;
	opcodes[0x69].action = std::bind(&CPU::ADC, this);

	opcodes[0x6A].Opcode = 0x6A;
	opcodes[0x6A].Cycles = 2;
	opcodes[0x6A].PageBoundary = false;
	opcodes[0x6A].RMW = true;
	opcodes[0x6A].Mode = Direct;
	opcodes[0x6A].action = std::bind(&CPU::ROR, this);

	opcodes[0x6B].Opcode = 0x6B;
	opcodes[0x6B].Cycles = 2;
	opcodes[0x6B].PageBoundary = false;
	opcodes[0x6B].RMW = false;
	opcodes[0x6B].Mode = Immediate;
	opcodes[0x6B].action = std::bind(&CPU::ARR, this);

	opcodes[0x6C].Opcode = 0x6C;
	opcodes[0x6C].Cycles = 5;
	opcodes[0x6C].PageBoundary = false;
	opcodes[0x6C].RMW = false;
	opcodes[0x6C].Mode = None;
	opcodes[0x6C].action = std::bind(&CPU::JMP, this);

	opcodes[0x6D].Opcode = 0x6D;
	opcodes[0x6D].Cycles = 4;
	opcodes[0x6D].PageBoundary = false;
	opcodes[0x6D].RMW = false;
	opcodes[0x6D].Mode = Absolute;
	opcodes[0x6D].action = std::bind(&CPU::ADC, this);

	opcodes[0x6E].Opcode = 0x6E;
	opcodes[0x6E].Cycles = 6;
	opcodes[0x6E].PageBoundary = false;
	opcodes[0x6E].RMW = true;
	opcodes[0x6E].Mode = Absolute;
	opcodes[0x6E].action = std::bind(&CPU::ROR, this);

	opcodes[0x6F].Opcode = -1;

	opcodes[0x70].Opcode = 0x70;
	opcodes[0x70].Cycles = 2;
	opcodes[0x70].PageBoundary = false;
	opcodes[0x70].RMW = false;
	opcodes[0x70].Mode = None;
	opcodes[0x70].action = std::bind(&CPU::BVS, this);

	opcodes[0x71].Opcode = 0x71;
	opcodes[0x71].Cycles = 5;
	opcodes[0x71].PageBoundary = true;
	opcodes[0x71].RMW = false;
	opcodes[0x71].Mode = IndirectY;
	opcodes[0x71].action = std::bind(&CPU::ADC, this);

	opcodes[0x72].Opcode = -1;

	opcodes[0x73].Opcode = -1;

	opcodes[0x74].Opcode = -1;

	opcodes[0x75].Opcode = 0x75;
	opcodes[0x75].Cycles = 4;
	opcodes[0x75].PageBoundary = false;
	opcodes[0x75].RMW = false;
	opcodes[0x75].Mode = ZeroPageX;
	opcodes[0x75].action = std::bind(&CPU::ADC, this);

	opcodes[0x76].Opcode = 0x76;
	opcodes[0x76].Cycles = 6;
	opcodes[0x76].PageBoundary = false;
	opcodes[0x76].RMW = true;
	opcodes[0x76].Mode = ZeroPageX;
	opcodes[0x76].action = std::bind(&CPU::ROR, this);

	opcodes[0x77].Opcode = -1;

	opcodes[0x78].Opcode = 0x78;
	opcodes[0x78].Cycles = 2;
	opcodes[0x78].PageBoundary = false;
	opcodes[0x78].RMW = false;
	opcodes[0x78].Mode = None;
	opcodes[0x78].action = std::bind(&CPU::SEI, this);

	opcodes[0x79].Opcode = 0x79;
	opcodes[0x79].Cycles = 4;
	opcodes[0x79].PageBoundary = true;
	opcodes[0x79].RMW = false;
	opcodes[0x79].Mode = AbsoluteY;
	opcodes[0x79].action = std::bind(&CPU::ADC, this);

	opcodes[0x7A].Opcode = 0x7A;
	opcodes[0x7A].Cycles = 2;
	opcodes[0x7A].PageBoundary = false;
	opcodes[0x7A].RMW = false;
	opcodes[0x7A].Mode = None;
	opcodes[0x7A].action = std::bind(&CPU::NOP, this);

	opcodes[0x7B].Opcode = -1;

	opcodes[0x7C].Opcode = -1;

	opcodes[0x7D].Opcode = 0x7D;
	opcodes[0x7D].Cycles = 4;
	opcodes[0x7D].PageBoundary = true;
	opcodes[0x7D].RMW = false;
	opcodes[0x7D].Mode = AbsoluteX;
	opcodes[0x7D].action = std::bind(&CPU::ADC, this);

	opcodes[0x7E].Opcode = 0x7E;
	opcodes[0x7E].Cycles = 7;
	opcodes[0x7E].PageBoundary = false;
	opcodes[0x7E].RMW = true;
	opcodes[0x7E].Mode = AbsoluteX;
	opcodes[0x7E].action = std::bind(&CPU::ROR, this);

	opcodes[0x7F].Opcode = -1;

	opcodes[0x80].Opcode = 0x80;
	opcodes[0x80].Cycles = 2;
	opcodes[0x80].PageBoundary = false;
	opcodes[0x80].RMW = false;
	opcodes[0x80].Mode = None;
	opcodes[0x80].action = std::bind(&CPU::SKB, this);

	opcodes[0x81].Opcode = 0x81;
	opcodes[0x81].Cycles = 6;
	opcodes[0x81].PageBoundary = false;
	opcodes[0x81].RMW = false;
	opcodes[0x81].Mode = IndirectX;
	opcodes[0x81].action = std::bind(&CPU::STA, this);

	opcodes[0x82].Opcode = 0x82;
	opcodes[0x82].Cycles = 2;
	opcodes[0x82].PageBoundary = false;
	opcodes[0x82].RMW = false;
	opcodes[0x82].Mode = None;
	opcodes[0x82].action = std::bind(&CPU::SKB, this);

	opcodes[0x83].Opcode = -1;

	opcodes[0x84].Opcode = 0x84;
	opcodes[0x84].Cycles = 3;
	opcodes[0x84].PageBoundary = false;
	opcodes[0x84].RMW = false;
	opcodes[0x84].Mode = ZeroPage;
	opcodes[0x84].action = std::bind(&CPU::STY, this);

	opcodes[0x85].Opcode = 0x85;
	opcodes[0x85].Cycles = 3;
	opcodes[0x85].PageBoundary = false;
	opcodes[0x85].RMW = false;
	opcodes[0x85].Mode = ZeroPage;
	opcodes[0x85].action = std::bind(&CPU::STA, this);

	opcodes[0x86].Opcode = 0x86;
	opcodes[0x86].Cycles = 3;
	opcodes[0x86].PageBoundary = false;
	opcodes[0x86].RMW = false;
	opcodes[0x86].Mode = ZeroPage;
	opcodes[0x86].action = std::bind(&CPU::STX, this);

	opcodes[0x87].Opcode = -1;

	opcodes[0x88].Opcode = 0x88;
	opcodes[0x88].Cycles = 2;
	opcodes[0x88].PageBoundary = false;
	opcodes[0x88].RMW = false;
	opcodes[0x88].Mode = None;
	opcodes[0x88].action = std::bind(&CPU::DEY, this);

	opcodes[0x89].Opcode = 0x89;
	opcodes[0x89].Cycles = 2;
	opcodes[0x89].PageBoundary = false;
	opcodes[0x89].RMW = false;
	opcodes[0x89].Mode = None;
	opcodes[0x89].action = std::bind(&CPU::SKB, this);

	opcodes[0x8A].Opcode = 0x8A;
	opcodes[0x8A].Cycles = 2;
	opcodes[0x8A].PageBoundary = false;
	opcodes[0x8A].RMW = true;
	opcodes[0x8A].Mode = None;
	opcodes[0x8A].action = std::bind(&CPU::TXA, this);

	opcodes[0x8B].Opcode = -1;

	opcodes[0x8C].Opcode = 0x8C;
	opcodes[0x8C].Cycles = 4;
	opcodes[0x8C].PageBoundary = false;
	opcodes[0x8C].RMW = false;
	opcodes[0x8C].Mode = Absolute;
	opcodes[0x8C].action = std::bind(&CPU::STY, this);

	opcodes[0x8D].Opcode = 0x8D;
	opcodes[0x8D].Cycles = 4;
	opcodes[0x8D].PageBoundary = false;
	opcodes[0x8D].RMW = false;
	opcodes[0x8D].Mode = Absolute;
	opcodes[0x8D].action = std::bind(&CPU::STA, this);

	opcodes[0x8E].Opcode = 0x8E;
	opcodes[0x8E].Cycles = 4;
	opcodes[0x8E].PageBoundary = false;
	opcodes[0x8E].RMW = false;
	opcodes[0x8E].Mode = Absolute;
	opcodes[0x8E].action = std::bind(&CPU::STX, this);

	opcodes[0x8F].Opcode = -1;

	opcodes[0x90].Opcode = 0x90;
	opcodes[0x90].Cycles = 2;
	opcodes[0x90].PageBoundary = false;
	opcodes[0x90].RMW = false;
	opcodes[0x90].Mode = None;
	opcodes[0x90].action = std::bind(&CPU::BCC, this);

	opcodes[0x91].Opcode = 0x91;
	opcodes[0x91].Cycles = 6;
	opcodes[0x91].PageBoundary = false;
	opcodes[0x91].RMW = false;
	opcodes[0x91].Mode = IndirectY;
	opcodes[0x91].action = std::bind(&CPU::STA, this);

	opcodes[0x92].Opcode = -1;

	opcodes[0x93].Opcode = -1;

	opcodes[0x94].Opcode = 0x94;
	opcodes[0x94].Cycles = 4;
	opcodes[0x94].PageBoundary = false;
	opcodes[0x94].RMW = false;
	opcodes[0x94].Mode = ZeroPageX;
	opcodes[0x94].action = std::bind(&CPU::STY, this);

	opcodes[0x95].Opcode = 0x95;
	opcodes[0x95].Cycles = 4;
	opcodes[0x95].PageBoundary = false;
	opcodes[0x95].RMW = false;
	opcodes[0x95].Mode = ZeroPageX;
	opcodes[0x95].action = std::bind(&CPU::STA, this);

	opcodes[0x96].Opcode = 0x96;
	opcodes[0x96].Cycles = 4;
	opcodes[0x96].PageBoundary = false;
	opcodes[0x96].RMW = false;
	opcodes[0x96].Mode = ZeroPageY;
	opcodes[0x96].action = std::bind(&CPU::STX, this);

	opcodes[0x97].Opcode = -1;

	opcodes[0x98].Opcode = 0x98;
	opcodes[0x98].Cycles = 2;
	opcodes[0x98].PageBoundary = false;
	opcodes[0x98].RMW = false;
	opcodes[0x98].Mode = None;
	opcodes[0x98].action = std::bind(&CPU::TYA, this);

	opcodes[0x99].Opcode = 0x99;
	opcodes[0x99].Cycles = 5;
	opcodes[0x99].PageBoundary = false;
	opcodes[0x99].RMW = false;
	opcodes[0x99].Mode = AbsoluteY;
	opcodes[0x99].action = std::bind(&CPU::STA, this);

	opcodes[0x9A].Opcode = 0x9A;
	opcodes[0x9A].Cycles = 2;
	opcodes[0x9A].PageBoundary = false;
	opcodes[0x9A].RMW = true;
	opcodes[0x9A].Mode = None;
	opcodes[0x9A].action = std::bind(&CPU::TXS, this);

	opcodes[0x9B].Opcode = -1;

	opcodes[0x9C].Opcode = -1;

	opcodes[0x9D].Opcode = 0x9D;
	opcodes[0x9D].Cycles = 5;
	opcodes[0x9D].PageBoundary = false;
	opcodes[0x9D].RMW = false;
	opcodes[0x9D].Mode = AbsoluteX;
	opcodes[0x9D].action = std::bind(&CPU::STA, this);

	opcodes[0x9E].Opcode = -1;

	opcodes[0x9F].Opcode = -1;

	opcodes[0xA0].Opcode = 0xA0;
	opcodes[0xA0].Cycles = 2;
	opcodes[0xA0].PageBoundary = false;
	opcodes[0xA0].RMW = false;
	opcodes[0xA0].Mode = Immediate;
	opcodes[0xA0].action = std::bind(&CPU::LDY, this);

	opcodes[0xA1].Opcode = 0xA1;
	opcodes[0xA1].Cycles = 6;
	opcodes[0xA1].PageBoundary = false;
	opcodes[0xA1].RMW = false;
	opcodes[0xA1].Mode = IndirectX;
	opcodes[0xA1].action = std::bind(&CPU::LDA, this);

	opcodes[0xA2].Opcode = 0xA2;
	opcodes[0xA2].Cycles = 2;
	opcodes[0xA2].PageBoundary = false;
	opcodes[0xA2].RMW = true;
	opcodes[0xA2].Mode = Immediate;
	opcodes[0xA2].action = std::bind(&CPU::LDX, this);

	opcodes[0xA3].Opcode = -1;

	opcodes[0xA4].Opcode = 0xA4;
	opcodes[0xA4].Cycles = 3;
	opcodes[0xA4].PageBoundary = false;
	opcodes[0xA4].RMW = false;
	opcodes[0xA4].Mode = ZeroPage;
	opcodes[0xA4].action = std::bind(&CPU::LDY, this);

	opcodes[0xA5].Opcode = 0xA5;
	opcodes[0xA5].Cycles = 3;
	opcodes[0xA5].PageBoundary = false;
	opcodes[0xA5].RMW = false;
	opcodes[0xA5].Mode = ZeroPage;
	opcodes[0xA5].action = std::bind(&CPU::LDA, this);

	opcodes[0xA6].Opcode = 0xA6;
	opcodes[0xA6].Cycles = 3;
	opcodes[0xA6].PageBoundary = false;
	opcodes[0xA6].RMW = true;
	opcodes[0xA6].Mode = ZeroPage;
	opcodes[0xA6].action = std::bind(&CPU::LDX, this);

	opcodes[0xA7].Opcode = -1;

	opcodes[0xA8].Opcode = 0xA8;
	opcodes[0xA8].Cycles = 2;
	opcodes[0xA8].PageBoundary = false;
	opcodes[0xA8].RMW = false;
	opcodes[0xA8].Mode = None;
	opcodes[0xA8].action = std::bind(&CPU::TAY, this);

	opcodes[0xA9].Opcode = 0xA9;
	opcodes[0xA9].Cycles = 2;
	opcodes[0xA9].PageBoundary = false;
	opcodes[0xA9].RMW = false;
	opcodes[0xA9].Mode = Immediate;
	opcodes[0xA9].action = std::bind(&CPU::LDA, this);

	opcodes[0xAA].Opcode = 0xAA;
	opcodes[0xAA].Cycles = 2;
	opcodes[0xAA].PageBoundary = false;
	opcodes[0xAA].RMW = true;
	opcodes[0xAA].Mode = None;
	opcodes[0xAA].action = std::bind(&CPU::TAX, this);

	opcodes[0xAB].Opcode = 0xAB;
	opcodes[0xAB].Cycles = 2;
	opcodes[0xAB].PageBoundary = false;
	opcodes[0xAB].RMW = false;
	opcodes[0xAB].Mode = Immediate;
	opcodes[0xAB].action = std::bind(&CPU::ATX, this);

	opcodes[0xAC].Opcode = 0xAC;
	opcodes[0xAC].Cycles = 4;
	opcodes[0xAC].PageBoundary = false;
	opcodes[0xAC].RMW = false;
	opcodes[0xAC].Mode = Absolute;
	opcodes[0xAC].action = std::bind(&CPU::LDY, this);

	opcodes[0xAD].Opcode = 0xAD;
	opcodes[0xAD].Cycles = 4;
	opcodes[0xAD].PageBoundary = false;
	opcodes[0xAD].RMW = false;
	opcodes[0xAD].Mode = Absolute;
	opcodes[0xAD].action = std::bind(&CPU::LDA, this);

	opcodes[0xAE].Opcode = 0xAE;
	opcodes[0xAE].Cycles = 4;
	opcodes[0xAE].PageBoundary = false;
	opcodes[0xAE].RMW = true;
	opcodes[0xAE].Mode = Absolute;
	opcodes[0xAE].action = std::bind(&CPU::LDX, this);

	opcodes[0xAF].Opcode = -1;

	opcodes[0xB0].Opcode = 0xB0;
	opcodes[0xB0].Cycles = 2;
	opcodes[0xB0].PageBoundary = false;
	opcodes[0xB0].RMW = false;
	opcodes[0xB0].Mode = None;
	opcodes[0xB0].action = std::bind(&CPU::BCS, this);

	opcodes[0xB1].Opcode = 0xB1;
	opcodes[0xB1].Cycles = 5;
	opcodes[0xB1].PageBoundary = true;
	opcodes[0xB1].RMW = false;
	opcodes[0xB1].Mode = IndirectY;
	opcodes[0xB1].action = std::bind(&CPU::LDA, this);

	opcodes[0xB2].Opcode = -1;

	opcodes[0xB3].Opcode = -1;

	opcodes[0xB4].Opcode = 0xB4;
	opcodes[0xB4].Cycles = 4;
	opcodes[0xB4].PageBoundary = false;
	opcodes[0xB4].RMW = false;
	opcodes[0xB4].Mode = ZeroPageX;
	opcodes[0xB4].action = std::bind(&CPU::LDY, this);

	opcodes[0xB5].Opcode = 0xB5;
	opcodes[0xB5].Cycles = 4;
	opcodes[0xB5].PageBoundary = false;
	opcodes[0xB5].RMW = false;
	opcodes[0xB5].Mode = ZeroPageX;
	opcodes[0xB5].action = std::bind(&CPU::LDA, this);

	opcodes[0xB6].Opcode = 0xB6;
	opcodes[0xB6].Cycles = 4;
	opcodes[0xB6].PageBoundary = false;
	opcodes[0xB6].RMW = true;
	opcodes[0xB6].Mode = ZeroPageY;
	opcodes[0xB6].action = std::bind(&CPU::LDX, this);

	opcodes[0xB7].Opcode = -1;

	opcodes[0xB8].Opcode = 0xB8;
	opcodes[0xB8].Cycles = 2;
	opcodes[0xB8].PageBoundary = false;
	opcodes[0xB8].RMW = false;
	opcodes[0xB8].Mode = None;
	opcodes[0xB8].action = std::bind(&CPU::CLV, this);

	opcodes[0xB9].Opcode = 0xB9;
	opcodes[0xB9].Cycles = 4;
	opcodes[0xB9].PageBoundary = true;
	opcodes[0xB9].RMW = false;
	opcodes[0xB9].Mode = AbsoluteY;
	opcodes[0xB9].action = std::bind(&CPU::LDA, this);

	opcodes[0xBA].Opcode = 0xBA;
	opcodes[0xBA].Cycles = 2;
	opcodes[0xBA].PageBoundary = false;
	opcodes[0xBA].RMW = false;
	opcodes[0xBA].Mode = None;
	opcodes[0xBA].action = std::bind(&CPU::TSX, this);

	opcodes[0xBB].Opcode = -1;

	opcodes[0xBC].Opcode = 0xBC;
	opcodes[0xBC].Cycles = 4;
	opcodes[0xBC].PageBoundary = true;
	opcodes[0xBC].RMW = false;
	opcodes[0xBC].Mode = AbsoluteX;
	opcodes[0xBC].action = std::bind(&CPU::LDY, this);

	opcodes[0xBD].Opcode = 0xBD;
	opcodes[0xBD].Cycles = 4;
	opcodes[0xBD].PageBoundary = true;
	opcodes[0xBD].RMW = false;
	opcodes[0xBD].Mode = AbsoluteX;
	opcodes[0xBD].action = std::bind(&CPU::LDA, this);

	opcodes[0xBE].Opcode = 0xBE;
	opcodes[0xBE].Cycles = 4;
	opcodes[0xBE].PageBoundary = true;
	opcodes[0xBE].RMW = true;
	opcodes[0xBE].Mode = AbsoluteY;
	opcodes[0xBE].action = std::bind(&CPU::LDX, this);

	opcodes[0xBF].Opcode = -1;

	opcodes[0xC0].Opcode = 0xC0;
	opcodes[0xC0].Cycles = 2;
	opcodes[0xC0].PageBoundary = false;
	opcodes[0xC0].RMW = false;
	opcodes[0xC0].Mode = Immediate;
	opcodes[0xC0].action = std::bind(&CPU::CPY, this);

	opcodes[0xC1].Opcode = 0xC1;
	opcodes[0xC1].Cycles = 6;
	opcodes[0xC1].PageBoundary = false;
	opcodes[0xC1].RMW = false;
	opcodes[0xC1].Mode = IndirectX;
	opcodes[0xC1].action = std::bind(&CPU::CMP, this);

	opcodes[0xC2].Opcode = 0xC2;
	opcodes[0xC2].Cycles = 2;
	opcodes[0xC2].PageBoundary = false;
	opcodes[0xC2].RMW = false;
	opcodes[0xC2].Mode = None;
	opcodes[0xC2].action = std::bind(&CPU::SKB, this);

	opcodes[0xC3].Opcode = -1;

	opcodes[0xC4].Opcode = 0xC4;
	opcodes[0xC4].Cycles = 3;
	opcodes[0xC4].PageBoundary = false;
	opcodes[0xC4].RMW = false;
	opcodes[0xC4].Mode = ZeroPage;
	opcodes[0xC4].action = std::bind(&CPU::CPY, this);

	opcodes[0xC5].Opcode = 0xC5;
	opcodes[0xC5].Cycles = 3;
	opcodes[0xC5].PageBoundary = false;
	opcodes[0xC5].RMW = false;
	opcodes[0xC5].Mode = ZeroPage;
	opcodes[0xC5].action = std::bind(&CPU::CMP, this);

	opcodes[0xC6].Opcode = 0xC6;
	opcodes[0xC6].Cycles = 5;
	opcodes[0xC6].PageBoundary = false;
	opcodes[0xC6].RMW = true;
	opcodes[0xC6].Mode = ZeroPage;
	opcodes[0xC6].action = std::bind(&CPU::DEC, this);

	opcodes[0xC7].Opcode = -1;

	opcodes[0xC8].Opcode = 0xC8;
	opcodes[0xC8].Cycles = 2;
	opcodes[0xC8].PageBoundary = false;
	opcodes[0xC8].RMW = false;
	opcodes[0xC8].Mode = None;
	opcodes[0xC8].action = std::bind(&CPU::INY, this);

	opcodes[0xC9].Opcode = 0xC9;
	opcodes[0xC9].Cycles = 2;
	opcodes[0xC9].PageBoundary = false;
	opcodes[0xC9].RMW = false;
	opcodes[0xC9].Mode = Immediate;
	opcodes[0xC9].action = std::bind(&CPU::CMP, this);

	opcodes[0xCA].Opcode = 0xCA;
	opcodes[0xCA].Cycles = 2;
	opcodes[0xCA].PageBoundary = false;
	opcodes[0xCA].RMW = true;
	opcodes[0xCA].Mode = None;
	opcodes[0xCA].action = std::bind(&CPU::DEX, this);

	opcodes[0xCB].Opcode = -1;

	opcodes[0xCC].Opcode = 0xCC;
	opcodes[0xCC].Cycles = 4;
	opcodes[0xCC].PageBoundary = false;
	opcodes[0xCC].RMW = false;
	opcodes[0xCC].Mode = Absolute;
	opcodes[0xCC].action = std::bind(&CPU::CPY, this);

	opcodes[0xCD].Opcode = 0xCD;
	opcodes[0xCD].Cycles = 4;
	opcodes[0xCD].PageBoundary = false;
	opcodes[0xCD].RMW = false;
	opcodes[0xCD].Mode = Absolute;
	opcodes[0xCD].action = std::bind(&CPU::CMP, this);

	opcodes[0xCE].Opcode = 0xCE;
	opcodes[0xCE].Cycles = 3;
	opcodes[0xCE].PageBoundary = false;
	opcodes[0xCE].RMW = true;
	opcodes[0xCE].Mode = Absolute;
	opcodes[0xCE].action = std::bind(&CPU::DEC, this);

	opcodes[0xCF].Opcode = -1;

	opcodes[0xD0].Opcode = 0xD0;
	opcodes[0xD0].Cycles = 2;
	opcodes[0xD0].PageBoundary = false;
	opcodes[0xD0].RMW = false;
	opcodes[0xD0].Mode = None;
	opcodes[0xD0].action = std::bind(&CPU::BNE, this);

	opcodes[0xD1].Opcode = 0xD1;
	opcodes[0xD1].Cycles = 5;
	opcodes[0xD1].PageBoundary = true;
	opcodes[0xD1].RMW = false;
	opcodes[0xD1].Mode = IndirectY;
	opcodes[0xD1].action = std::bind(&CPU::CMP, this);

	opcodes[0xD2].Opcode = -1;

	opcodes[0xD3].Opcode = -1;

	opcodes[0xD4].Opcode = -1;

	opcodes[0xD5].Opcode = 0xD5;
	opcodes[0xD5].Cycles = 4;
	opcodes[0xD5].PageBoundary = false;
	opcodes[0xD5].RMW = false;
	opcodes[0xD5].Mode = ZeroPageX;
	opcodes[0xD5].action = std::bind(&CPU::CMP, this);

	opcodes[0xD6].Opcode = 0xD6;
	opcodes[0xD6].Cycles = 6;
	opcodes[0xD6].PageBoundary = false;
	opcodes[0xD6].RMW = true;
	opcodes[0xD6].Mode = ZeroPageX;
	opcodes[0xD6].action = std::bind(&CPU::DEC, this);

	opcodes[0xD7].Opcode = -1;

	opcodes[0xD8].Opcode = 0xD8;
	opcodes[0xD8].Cycles = 2;
	opcodes[0xD8].PageBoundary = false;
	opcodes[0xD8].RMW = false;
	opcodes[0xD8].Mode = None;
	opcodes[0xD8].action = std::bind(&CPU::CLD, this);

	opcodes[0xD9].Opcode = 0xD9;
	opcodes[0xD9].Cycles = 4;
	opcodes[0xD9].PageBoundary = true;
	opcodes[0xD9].RMW = false;
	opcodes[0xD9].Mode = AbsoluteY;
	opcodes[0xD9].action = std::bind(&CPU::CMP, this);

	opcodes[0xDA].Opcode = 0xDA;
	opcodes[0xDA].Cycles = 2;
	opcodes[0xDA].PageBoundary = false;
	opcodes[0xDA].RMW = false;
	opcodes[0xDA].Mode = None;
	opcodes[0xDA].action = std::bind(&CPU::NOP, this);

	opcodes[0xDB].Opcode = -1;

	opcodes[0xDC].Opcode = -1;

	opcodes[0xDD].Opcode = 0xDD;
	opcodes[0xDD].Cycles = 4;
	opcodes[0xDD].PageBoundary = true;
	opcodes[0xDD].RMW = false;
	opcodes[0xDD].Mode = AbsoluteX;
	opcodes[0xDD].action = std::bind(&CPU::CMP, this);

	opcodes[0xDE].Opcode = 0xDE;
	opcodes[0xDE].Cycles = 7;
	opcodes[0xDE].PageBoundary = false;
	opcodes[0xDE].RMW = true;
	opcodes[0xDE].Mode = AbsoluteX;
	opcodes[0xDE].action = std::bind(&CPU::DEC, this);

	opcodes[0xDF].Opcode = -1;

	opcodes[0xE0].Opcode = 0xE0;
	opcodes[0xE0].Cycles = 2;
	opcodes[0xE0].PageBoundary = false;
	opcodes[0xE0].RMW = false;
	opcodes[0xE0].Mode = Immediate;
	opcodes[0xE0].action = std::bind(&CPU::CPX, this);

	opcodes[0xE1].Opcode = 0xE1;
	opcodes[0xE1].Cycles = 6;
	opcodes[0xE1].PageBoundary = false;
	opcodes[0xE1].RMW = false;
	opcodes[0xE1].Mode = IndirectX;
	opcodes[0xE1].action = std::bind(&CPU::SBC, this);

	opcodes[0xE2].Opcode = 0xE2;
	opcodes[0xE2].Cycles = 2;
	opcodes[0xE2].PageBoundary = false;
	opcodes[0xE2].RMW = false;
	opcodes[0xE2].Mode = None;
	opcodes[0xE2].action = std::bind(&CPU::SKB, this);

	opcodes[0xE3].Opcode = -1;

	opcodes[0xE4].Opcode = 0xE4;
	opcodes[0xE4].Cycles = 3;
	opcodes[0xE4].PageBoundary = false;
	opcodes[0xE4].RMW = false;
	opcodes[0xE4].Mode = ZeroPage;
	opcodes[0xE4].action = std::bind(&CPU::CPX, this);

	opcodes[0xE5].Opcode = 0xE5;
	opcodes[0xE5].Cycles = 3;
	opcodes[0xE5].PageBoundary = false;
	opcodes[0xE5].RMW = false;
	opcodes[0xE5].Mode = ZeroPage;
	opcodes[0xE5].action = std::bind(&CPU::SBC, this);

	opcodes[0xE6].Opcode = 0xE6;
	opcodes[0xE6].Cycles = 5;
	opcodes[0xE6].PageBoundary = false;
	opcodes[0xE6].RMW = true;
	opcodes[0xE6].Mode = ZeroPage;
	opcodes[0xE6].action = std::bind(&CPU::INC, this);

	opcodes[0xE7].Opcode = -1;

	opcodes[0xE8].Opcode = 0xE8;
	opcodes[0xE8].Cycles = 2;
	opcodes[0xE8].PageBoundary = false;
	opcodes[0xE8].RMW = false;
	opcodes[0xE8].Mode = None;
	opcodes[0xE8].action = std::bind(&CPU::INX, this);

	opcodes[0xE9].Opcode = 0xE9;
	opcodes[0xE9].Cycles = 2;
	opcodes[0xE9].PageBoundary = false;
	opcodes[0xE9].RMW = false;
	opcodes[0xE9].Mode = Immediate;
	opcodes[0xE9].action = std::bind(&CPU::SBC, this);

	opcodes[0xEA].Opcode = 0xEA;
	opcodes[0xEA].Cycles = 2;
	opcodes[0xEA].PageBoundary = false;
	opcodes[0xEA].RMW = false;
	opcodes[0xEA].Mode = None;
	opcodes[0xEA].action = std::bind(&CPU::NOP, this);

	opcodes[0xEB].Opcode = 0xEB;
	opcodes[0xEB].Cycles = 2;
	opcodes[0xEB].PageBoundary = false;
	opcodes[0xEB].RMW = false;
	opcodes[0xEB].Mode = Immediate;
	opcodes[0xEB].action = std::bind(&CPU::SBC, this);

	opcodes[0xEC].Opcode = 0xEC;
	opcodes[0xEC].Cycles = 4;
	opcodes[0xEC].PageBoundary = false;
	opcodes[0xEC].RMW = false;
	opcodes[0xEC].Mode = Absolute;
	opcodes[0xEC].action = std::bind(&CPU::CPX, this);

	opcodes[0xED].Opcode = 0xED;
	opcodes[0xED].Cycles = 4;
	opcodes[0xED].PageBoundary = false;
	opcodes[0xED].RMW = false;
	opcodes[0xED].Mode = Absolute;
	opcodes[0xED].action = std::bind(&CPU::SBC, this);

	opcodes[0xEE].Opcode = 0xEE;
	opcodes[0xEE].Cycles = 6;
	opcodes[0xEE].PageBoundary = false;
	opcodes[0xEE].RMW = true;
	opcodes[0xEE].Mode = Absolute;
	opcodes[0xEE].action = std::bind(&CPU::INC, this);

	opcodes[0xEF].Opcode = -1;

	opcodes[0xF0].Opcode = 0xF0;
	opcodes[0xF0].Cycles = 2;
	opcodes[0xF0].PageBoundary = false;
	opcodes[0xF0].RMW = false;
	opcodes[0xF0].Mode = None;
	opcodes[0xF0].action = std::bind(&CPU::BEQ, this);

	opcodes[0xF1].Opcode = 0xF1;
	opcodes[0xF1].Cycles = 5;
	opcodes[0xF1].PageBoundary = true;
	opcodes[0xF1].RMW = false;
	opcodes[0xF1].Mode = IndirectY;
	opcodes[0xF1].action = std::bind(&CPU::SBC, this);

	opcodes[0xF2].Opcode = -1;

	opcodes[0xF3].Opcode = -1;

	opcodes[0xF4].Opcode = -1;

	opcodes[0xF5].Opcode = 0xF5;
	opcodes[0xF5].Cycles = 4;
	opcodes[0xF5].PageBoundary = false;
	opcodes[0xF5].RMW = false;
	opcodes[0xF5].Mode = ZeroPageX;
	opcodes[0xF5].action = std::bind(&CPU::SBC, this);

	opcodes[0xF6].Opcode = 0xF6;
	opcodes[0xF6].Cycles = 6;
	opcodes[0xF6].PageBoundary = false;
	opcodes[0xF6].RMW = true;
	opcodes[0xF6].Mode = ZeroPageX;
	opcodes[0xF6].action = std::bind(&CPU::INC, this);

	opcodes[0xF7].Opcode = -1;

	opcodes[0xF8].Opcode = 0xF8;
	opcodes[0xF8].Cycles = 2;
	opcodes[0xF8].PageBoundary = false;
	opcodes[0xF8].RMW = false;
	opcodes[0xF8].Mode = None;
	opcodes[0xF8].action = std::bind(&CPU::SED, this);

	opcodes[0xF9].Opcode = 0xF9;
	opcodes[0xF9].Cycles = 4;
	opcodes[0xF9].PageBoundary = true;
	opcodes[0xF9].RMW = false;
	opcodes[0xF9].Mode = AbsoluteY;
	opcodes[0xF9].action = std::bind(&CPU::SBC, this);

	opcodes[0xFA].Opcode = 0xFA;
	opcodes[0xFA].Cycles = 2;
	opcodes[0xFA].PageBoundary = false;
	opcodes[0xFA].RMW = false;
	opcodes[0xFA].Mode = None;
	opcodes[0xFA].action = std::bind(&CPU::NOP, this);

	opcodes[0xFB].Opcode = -1;

	opcodes[0xFC].Opcode = -1;

	opcodes[0xFD].Opcode = 0xFD;
	opcodes[0xFD].Cycles = 4;
	opcodes[0xFD].PageBoundary = true;
	opcodes[0xFD].RMW = false;
	opcodes[0xFD].Mode = AbsoluteX;
	opcodes[0xFD].action = std::bind(&CPU::SBC, this);

	opcodes[0xFE].Opcode = 0xFE;
	opcodes[0xFE].Cycles = 7;
	opcodes[0xFE].PageBoundary = false;
	opcodes[0xFE].RMW = true;
	opcodes[0xFE].Mode = AbsoluteX;
	opcodes[0xFE].action = std::bind(&CPU::INC, this);

	opcodes[0xFF].Opcode = -1;

#pragma endregion



	/*InitializeMemoryMap*/

	Byte* memory_pointer = memoryHandler.GetMemoryAddress();
	memoryHandler.SetReadHandler(0x0000, 0x1FFF,
		[memory_pointer](int address) {
			return memory_pointer[address & 0x07FF];
		});
	memoryHandler.SetReadHandler(0x2000, 0x3FFF,
		[this](int address) {
			return emulator->ppu->ReadRegister((address & 0x7) - 0x2000);
		});
	memoryHandler.SetReadHandler(0x4000, 0x4017,
		[this](int address) {
			return ReadIORegister(address);
		});

	memoryHandler.SetWriteHandler(0x0000, 0x1FFF,
		[memory_pointer](int address, Byte value) {
			memory_pointer[address & 0x07FF] = value;
		});
	memoryHandler.SetWriteHandler(0x2000, 0x3FFF,
		[this](int address, Byte value) {
			emulator->ppu->WriteRegister((address & 0x7) - 0x2000, value);
		});
	memoryHandler.SetWriteHandler(0x4000, 0x4017,
		[this](int address, Byte value) {
			WriteIORegister(address, value);
		});

	emulator->Mapper->InitializeMemoryMap(this);

	/*init Register.s*/
	Register.setA(0);
	Register.setX(0);
	Register.setY(0);
	Register.SP = 0xFD;
	Register.setP(0x24);
	Register.PC = ReadWord(InterruptHandlerOffsets[InterruptType::RESET]);
}


void CPU::ExecuteSingleInstruction()
{
	static int ops[100000];
	static QWord opi = 0;

	for (int i = 0; i < 2; i++)
	{
		if (interrupts[i])
		{
			PushWord(Register.PC);
			Push(Register.getP());
			Register.PC = ReadWord(InterruptHandlerOffsets[i]);
			Register.P.InterruptDisabled = true;
			interrupts[i] = false;
			return;
		}
	}
	currentInstruction = NextByte();

	cycle += opcodes[currentInstruction].Cycles;

	ResetInstructionAddressingMode();

	std::function<void(void)> op = opcodes[currentInstruction].action;
	if (op == nullptr)
		throw "NULL opcode executing!";
	op();
	opi++;
	if (opi < 100000)
	{
		ops[opi] = currentInstruction;
	}
	
}
