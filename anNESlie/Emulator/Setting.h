#pragma once
#include <map>


class Setting
{
public:
	int controller;
	char keyBindings[128];

private:
	Setting();
	static Setting* setting;
	static void clearSetting();

public:
	static Setting* ReadSetting();
	static bool SaveSetting();
	static void RestoreDefault();
};