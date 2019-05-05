#pragma once
#include "Controller.h"

namespace Controller
{
	class NES001Controller :public Controller
	{
	private:
		int data;
		int serialData;
		bool strobing;

	public:
		NES001Controller();

		virtual void Strobe(bool on);
		virtual int ReadState();
		virtual void PressKey(int keyCode);
		virtual void ReleaseKey(int keyCode);
	};
}