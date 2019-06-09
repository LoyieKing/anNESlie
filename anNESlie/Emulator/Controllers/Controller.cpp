#include "Controller.h"
#include "NES001Controller.h"

//const int Controller::Controller::key_number = 0;
const  Controller::Key const*  Controller::Controller::keys = nullptr;


int Controller::Controller::GetKeyNumber()
{
	return key_number;
}

Controller::Key const* Controller::Controller::GetKeys()
{
	return keys;
}

Controller::Controller::~Controller()
{
}

Controller::Controller* Controller::GetController(int id)
{
	switch (id)
	{
	case 0:
		return new NES001Controller();
	default:
		return nullptr;
	}
}
