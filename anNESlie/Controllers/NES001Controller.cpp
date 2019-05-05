#include "NES001Controller.h"

Controller::NES001Controller::NES001Controller()
{
	data = 0;
	serialData = 0;
	strobing = 0;
}

void Controller::NES001Controller::Strobe(bool on)
{
	serialData = data;
	strobing = true;
}

int Controller::NES001Controller::ReadState()
{
	Byte ret = serialData & 0x80;
	if (!strobing)
	{
		serialData <<= 1;
		serialData &= 0xFF;
	}
	return ret;
}

void Controller::NES001Controller::PressKey(int keyCode)
{

}

void Controller::NES001Controller::ReleaseKey(int keyCode)
{
}
