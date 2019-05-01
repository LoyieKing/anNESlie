#pragma once

/*08bits    1bytes    0xFF*/
typedef unsigned char Byte;
typedef signed char SByte;

/*16bits    2bytes    0xFFFF*/
typedef short Word;

/*32bits    4bytes    0xFFFFFFFF*/
typedef long DWord;

/*64bits    8bytes    0xFFFFFFFFFFFFFFFF*/
typedef long long QWord;


/*CPU Interrupt Type*/
enum InterruptType
{
	NMI, IRQ, RESET
};

Word InterruptHandlerOffsets[] = {
	0xFFFA, 0xFFFE, 0xFFFC
};