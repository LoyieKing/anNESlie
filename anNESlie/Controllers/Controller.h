#pragma once

namespace Controller
{
	class Controller
	{
	public:
		virtual void Strobe(bool on) = 0;
		virtual int ReadState() = 0;
		virtual void PressKey(int keyCode) = 0;
		virtual void ReleaseKey(int keyCode) = 0;
	};
}