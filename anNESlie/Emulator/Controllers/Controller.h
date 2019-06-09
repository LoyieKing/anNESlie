#pragma once
#include "../Types.h"

namespace Controller
{
	struct Key
	{
		int KeyCode;
		const char* KeyName;
	};

	class Controller
	{
	protected:
		const static int key_number;
		const static Key const* keys;

	public:
		const static int ControllerID;
		const static char* ControllerName;

		virtual int GetKeyNumber();
		virtual Key const* GetKeys();

		virtual void Strobe(bool on) = 0;
		virtual Byte ReadState() = 0;
		virtual void PressKey(int keyCode) = 0;
		virtual void ReleaseKey(int keyCode) = 0;

		virtual ~Controller();
	};

	Controller* GetController(int id);
}