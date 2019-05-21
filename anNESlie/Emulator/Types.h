#pragma once

/*08bits    1bytes    0xFF*/
typedef unsigned char Byte;
typedef signed char SByte;

/*16bits    2bytes    0xFFFF*/
typedef unsigned short Word;

/*32bits    4bytes    0xFFFFFFFF*/
typedef unsigned long DWord;

/*64bits    8bytes    0xFFFFFFFFFFFFFFFF*/
typedef unsigned long long QWord;


/*CPU Interrupt Type*/
enum InterruptType
{
	NMI, IRQ, RESET
};

const int GAME_WIDTH = 256;
const int GAME_HEIGHT = 240;

struct Color
{
	Byte R;
	Byte G;
	Byte B;

	Color();
	Color(DWord RGB);
};