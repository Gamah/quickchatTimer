#include "pch.h"
#include "quickchatTimer.h"


BAKKESMOD_PLUGIN(quickchatTimer, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;



void quickchatTimer::Render(CanvasWrapper canvas) {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    SettingsWrapper x;
    VideoSettings y = x.GetVideoSettings();

    std::string z = y.Resolution;

    float width, height;

    std::stringstream ss(z);
    std::string token;

    // Get the width
    std::getline(ss, token, 'x');
    width = std::stof(token);

    // Get the height
    std::getline(ss, token);
    height = std::stof(token);

    height = height * 0.395;

    int out = milliseconds - start;
    int qctime = 1750;

    if (out > qctime || qctime - out > qctime) {
        out = 0;
    }

    float barstart = width * 0.014; // Left edge of the bar
    float barstop = width * 0.18;  // Initial right edge of the bar
    float progress = (float)out / (float)qctime;

    // Calculate the shrinking bar's current right edge
    float barend = barstop - (progress * (barstop - barstart));

    // Defines colors in RGBA 0-255
    LinearColor colors;
    colors.R = 0;
    colors.G = 255;
    colors.B = 0;
    colors.A = 128;
    canvas.SetColor(colors);

    if (progress != 0) {
        colors.R = progress * 255;
        colors.G = (1.0 - progress) * 255;
        canvas.SetColor(colors);

        // Draw the rectangle shrinking from right to left
        canvas.DrawRect(Vector2F{ barstart, height }, Vector2F{ barend, height + (float(height * 0.55)) });
    }


	//debug
	/*
	canvas.SetPosition(Vector2F{ 0, height - 80 });
	canvas.DrawString(std::to_string(barstart), 10.0, 10.0, false);
	
	canvas.SetPosition(Vector2F{ 0, height  });
	canvas.DrawString(std::to_string(barstop), 10.0, 10.0, false);

	canvas.SetPosition(Vector2F{ 0, height + 80 });
	canvas.DrawString(std::to_string(barend), 10.0, 10.0, false);

	canvas.SetPosition(Vector2F{ 0, height + 80 });
	canvas.DrawString(std::to_string(progress), 10.0, 10.0, false);
	*/

	



}

void quickchatTimer::onLoad()
{
	_globalCvarManager = cvarManager;
	

	gameWrapper->HookEvent("Function TAGame.GFxData_Chat_TA.OnPressChatPreset", [this](std::string eventName) {
		
		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

		start = milliseconds;
		
		
	});

	gameWrapper->HookEvent("Function TAGame.GFxData_Chat_TA.InternalAddMessage", [this](std::string eventName) {

			auto now = std::chrono::system_clock::now();
			auto duration = now.time_since_epoch();
			auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

			start = 9999.0;

	});

	gameWrapper->RegisterDrawable([this](CanvasWrapper canvas) {
		Render(canvas);
		});
}
