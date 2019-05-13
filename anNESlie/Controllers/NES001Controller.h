#pragma once
#include "Controller.h"

namespace Controller
{

	// bit:   	 7     6     5     4     3     2     1     0
	// button:	 A     B   Select Start  Up  Down   Left  Right
	class NES001Controller :public Controller
	{
	private:
		int data;
		int serialData;
		bool strobing;

	protected:
		const static Key keys[];
	public:
		NES001Controller();

		virtual void Strobe(bool on);
		virtual int ReadState();
		virtual void PressKey(int keyCode);
		virtual void ReleaseKey(int keyCode);
	};
}