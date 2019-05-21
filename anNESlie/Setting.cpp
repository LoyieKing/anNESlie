#include "Setting.h"
#include <iostream>
#include <map>

Setting* Setting::setting = nullptr;

Setting::Setting()
{
}

void Setting::clearSetting()
{
	if (setting == nullptr)
		setting = new Setting();
	setting->controller = -1;
	memset(setting->keyBindings, -1, sizeof(setting->keyBindings));
}

Setting* Setting::ReadSetting()
{
	FILE* file = fopen("setting.dat", "r");
	if (!file)
	{
		RestoreDefault();
		return setting;
	}
	clearSetting();
	char item[64];
	while (fscanf(file, "%s=", item) != EOF)
	{
		if (strcmp("Controller", item) == 0)
		{
			fscanf(file, "%d", &setting->controller);
		}
		else if (strcmp("KeyBinding", item) == 0)
		{
			int keyboard;
			int controller;
			fscanf(file, "%d,%d", &keyboard, &controller);
			setting->keyBindings[keyboard] = controller;
		}
	}
	fclose(file);

	if (setting->controller == -1)//setting file is not right
		RestoreDefault();

	return setting;
}

bool Setting::SaveSetting()
{
	if (setting == nullptr)
		return false;
	FILE* file = fopen("setting.dat", "w");
	if (!file)
		return false;
	fprintf(file, "Controller=%d\n", setting->controller);
	for (int i = 0; i < 128; i++)
	{
		if (setting->keyBindings[i] == -1)
			continue;
		fprintf(file, "KeyBinding=%d,%d\n", i, setting->keyBindings[i]);
	}
	fclose(file);
}

void Setting::RestoreDefault()
{
	clearSetting();
	setting->controller = 0;
	setting->keyBindings[39] = 0;			//VK_RIGHT = Right
	setting->keyBindings[37] = 1;			//VK_LEFT = Left
	setting->keyBindings[40] = 2;			//VK_DOWN = Down
	setting->keyBindings[38] = 3;			//VK_UP = Up
	setting->keyBindings[13] = 4;			//VK_RETURN (Enter) = Start
	setting->keyBindings[161] = 5;			//VK_RSHIFT = Select
	setting->keyBindings[90] = 6;			//Z = B
	setting->keyBindings[88] = 7;			//X = A
}
