#include "common.h"
#include "mcu.h"
#include "power.h"
#include "graphics/screen.h"
#include "theme.h"

struct colour TopScreen_bar_colour;

void drawBatteryStatus(void)
{
	u8 batteryPercent = 0;

	if (R_SUCCEEDED(MCU_GetBatteryLevel(&batteryPercent)))
	{
		if (batteryPercent == 0)
			screen_draw_texture(TEXTURE_BATTERY_0, 280, 1);
		else if (batteryPercent > 0 && batteryPercent <= 15)
			screen_draw_texture(TEXTURE_BATTERY_15, 280, 1);
		else if (batteryPercent > 15 && batteryPercent <= 28)
			screen_draw_texture(TEXTURE_BATTERY_28, 280, 1);
		else if (batteryPercent > 28 && batteryPercent <= 43)
			screen_draw_texture(TEXTURE_BATTERY_43, 280, 1);
		else if (batteryPercent > 43 && batteryPercent <= 57)
			screen_draw_texture(TEXTURE_BATTERY_57, 280, 1);
		else if (batteryPercent > 57 && batteryPercent <= 71)
			screen_draw_texture(TEXTURE_BATTERY_71, 280, 1);
		else if (batteryPercent > 71 && batteryPercent <= 99)
			screen_draw_texture(TEXTURE_BATTERY_85, 280, 1);
		else if (batteryPercent == 100)
			screen_draw_texture(TEXTURE_BATTERY_100, 280, 1);
	}

	u8 batteryState = false; // boolean that represnets charging state

	if (R_SUCCEEDED(PTMU_GetBatteryChargeState(&batteryState)))
	{
		if (batteryState)
			screen_draw_texture(TEXTURE_BATTERY_CHARGE, 282, 3);
	}
		
	screen_draw_stringf(300, 2, 0.44f, 0.44f, RGBA8(TopScreen_bar_colour.r, TopScreen_bar_colour.g, TopScreen_bar_colour.b, 255), "%2d%%", batteryPercent);
}
