#include "NES001Controller.h"

const int Controller::NES001Controller::ControllerID = 0;
const char* Controller::NES001Controller::ControllerName = "NES Default Controller";
const int Controller::NES001Controller::key_number = 8;
const Controller::Key Controller::NES001Controller::keys[] =
{
	{0,"Right"},
	{1,"Left"},
	{2,"Down"},
	{3,"Up"},
	{4,"Start"},
	{5,"Select"},
	{6,"B"},
	{7,"A"}
};


Controller::NES001Controller::NES001Controller()
{
	data = 0;
	serialData = 0;
	strobing = false;
}


void Controller::NES001Controller::Strobe(bool on)
{
	serialData = data;
	strobing = on;
}

Byte Controller::NES001Controller::ReadState()
{
	Byte ret = (serialData & 0x80) > 0;
	if (!strobing)
	{
		serialData <<= 1;
	}
	return ret;
}

void Controller::NES001Controller::PressKey(int keyIndex)
{
	if (keyIndex < 0 || keyIndex>=key_number)
		return;
	data |= 1 << keys[keyIndex].KeyCode;
}

void Controller::NES001Controller::ReleaseKey(int keyIndex)
{
	if (keyIndex < 0 || keyIndex>=key_number)
		return;
	data &= ~(1 << keys[keyIndex].KeyCode);
}
