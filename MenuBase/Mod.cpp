#include "Mod.h"

#include "menu/Draw.h"
#include "menu/Menu.h"
#include "menu/CleoMenu.h"

#include "Log.h"
#include "ModConfig.h"
#include "Peds.h"
#include "Vehicles.h"
#include "Input.h"

#include "windows/WindowTest.h"

extern CVector2D *m_vecCachedPos;

const char* Mod::m_Version = "1.0.0";
unsigned int Mod::m_TimePassed = 0;
bool Mod::m_Enabled = true;

int testButton = 0;

void Mod::Update(int dt)
{
    //Log::file << "Mod::Update" << std::endl;

    m_TimePassed += dt;

    while (Draw::m_DrawItems.size() > 0)
    {
        auto dw = Draw::m_DrawItems[0];
        Draw::m_DrawItems.erase(std::find(Draw::m_DrawItems.begin(), Draw::m_DrawItems.end(), dw));
        delete dw;
    }

    //

    Log::Level(LOG_LEVEL::LOG_UPDATE) << "peds ------------------" << std::endl;

    Peds::Update(dt);

    Log::Level(LOG_LEVEL::LOG_UPDATE) << "vehicles" << std::endl;

    Vehicles::Update(dt);

    Log::Level(LOG_LEVEL::LOG_UPDATE) << "menu" << std::endl;

    Menu::Update(dt);
    
    Menu::Draw();

    Log::Level(LOG_LEVEL::LOG_UPDATE) << "input" << std::endl;

    Input::Update(dt);

    Mod::ProcessMenuButtons(dt);

    //

    //Draw::DrawBoxWithText(2, 1, 2, {0, 0}, {50, 50}, {255, 0, 0, 255}, {0, 0, 0, 255});

    //

    if (Menu::m_DrawCursor)
    {
        Draw::DrawText(2, m_vecCachedPos->x, m_vecCachedPos->y, CVector2D(20, 300), CRGBA(255, 255, 0));
        Draw::DrawText(2, (int)Menu::m_MenuOffset.x, (int)Menu::m_MenuOffset.y, CVector2D(20, 320), CRGBA(255, 255, 0));
        Draw::DrawText(1, Draw::m_DrawItems.size(), 0, CVector2D(20, 340), CRGBA(255, 255, 0));
        Draw::DrawText(1, dt, 0, CVector2D(20, 360), CRGBA(255, 255, 0));
    }

    Log::Level(LOG_LEVEL::LOG_UPDATE) << "end ---------" << std::endl;
}

void Mod::Init()
{
    
}

int Mod::GetRandomNumber(int min, int max)
{
    int n = max - min + 1;
    int remainder = RAND_MAX % n;
    int x;
    do{
        x = rand();
    }while (x >= RAND_MAX - remainder);
    return min + x % n;
}

bool Mod::CalculateProbability(float chance)
{
    int i = GetRandomNumber(0, 100);
    return i <= (int)(chance * 100.0f);
}

void Mod::ProcessMenuButtons(int dt)
{
    if (Input::GetTouchIdState(1) && Input::GetTouchIdState(6))
    {
        if (Input::GetTouchIdPressTime(6) > 500)
        {
            WindowTest::Create();
        }
    }
}

void Mod::ToggleMod(bool enabled)
{
    Log::Level(LOG_LEVEL::LOG_BOTH) << "Toggle mod: " << (enabled ? "Enabled" : "Disabled") << std::endl;

    m_Enabled = enabled;
}