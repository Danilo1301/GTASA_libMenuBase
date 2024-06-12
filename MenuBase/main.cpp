#include "pch.h"

#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

#include "Log.h"
#include "Mod.h"
#include "ModConfig.h"

#include "opcodes.h"

// ---------------------------------------

MYMODCFG(net.danilo1301.menuBase, MenuBase, 1.0.0, Danilo1301)

// ---------------------------------------

// CLEO 2.0.1.3
#include "cleo.h"
cleo_ifs_t* cleo = NULL;

#include "cleoaddon.h"
cleo_addon_ifs_t* cleoaddon = NULL;

// SAUtils 1.6 (but it says 1.5.2)
#include "isautils.h"
ISAUtils* sautils = NULL;

// BASS
IBASS* BASS = NULL;

// ---------------------------------------

CVector2D *m_vecCachedPos;
uintptr_t* pVehiclePool = 0;
uintptr_t* pPedPool = 0;

int (*ScreenGetWidth)();
int (*ScreenGetHeight)();
int (*GetVehicleRef)(int);
void* (*GetVehicleFromRef)(int);
int (*GetPedRef)(int);
void* (*GetPedFromRef)(int);
void (*UpdateCompareFlag)(void*, uint8_t);

CCamera* camera;
bool* userPaused;
bool* codePaused;

// ---------------------------------------

ConfigEntry* cfgMenuOffsetX = NULL;

//---------------------------------------------------------------------------------------------------
//test

const char* optionsTest[] = {
    "test1",
    "test2"
};
void TestChanged(int oldVal, int newVal, void* data)
{
    Log::Level(LOG_LEVEL::LOG_BOTH) << "TestChanged - changed to " << newVal << std::endl;
}

//---------------------------------------------------------------------------------------------------

extern "C" void OnModPreLoad()
{
    ModConfig::MakePaths();

    char logPath[512];
	sprintf(logPath, "%s/menuBase/", aml->GetConfigPath());
    Log::Open(logPath, "menuBase");
    
    Log::Level(LOG_LEVEL::LOG_BOTH) << "Preload()" << std::endl;
    Log::Level(LOG_LEVEL::LOG_BOTH) << "AML headers: 1.0.3.1" << std::endl;


    logger->SetTag("MenuBase");

    logger->Info("Preload");
}

extern "C" void OnModLoad()
{
    Log::Level(LOG_LEVEL::LOG_BOTH) << "Load()" << std::endl;

    cfgMenuOffsetX = cfg->Bind("menu_offset_x", -195, "General"); cfg->ClearLast();

    Menu::m_MenuOffset.x = (float)cfgMenuOffsetX->GetInt();

    cfg->Bind("Author", "", "About")->SetString("Danilo1301"); cfg->ClearLast();
    cfg->Bind("Discord", "", "About")->SetString("https://discord.gg/mkCDRf4zJA"); cfg->ClearLast();
    //cfg->Bind("GitHub", "", "About")->SetString("https://github.com/Danilo1301/"); cfg->ClearLast();
    cfg->Save();

    
    //CLEO
    Log::Level(LOG_LEVEL::LOG_BOTH) << "Loading CLEO..." << std::endl;
    cleo = (cleo_ifs_t*)GetInterface("CLEO");
    if (cleo)
    {
        Log::Level(LOG_LEVEL::LOG_BOTH) << "CLEO loaded" << std::endl;
    }

    //CLEOAddon
    Log::Level(LOG_LEVEL::LOG_BOTH) << "Loading CLEOAddon..." << std::endl;
    if((cleoaddon = (cleo_addon_ifs_t*)GetInterface("CLEOAddon")))
    {
        Log::Level(LOG_LEVEL::LOG_BOTH) << "CLEOAddon loaded" << std::endl;
    } else {
        Log::Level(LOG_LEVEL::LOG_BOTH) << "CLEOAddon was not loaded" << std::endl;
    }

    //SAUtils
    Log::Level(LOG_LEVEL::LOG_BOTH) << "Loading SAUtils..." << std::endl;
    if (!(sautils = (ISAUtils*)GetInterface("SAUtils")))
    {
        Log::Level(LOG_LEVEL::LOG_BOTH) << "SAUtils was not loaded" << std::endl;
    }
    else {
        Log::Level(LOG_LEVEL::LOG_BOTH) << "SAUtils loaded" << std::endl;

        sautils->AddClickableItem(eTypeOfSettings::SetType_Mods, "Test", 0, 0, sizeofA(optionsTest) - 1, optionsTest, TestChanged);
    }

    //BASS
    //https://github.com/AndroidModLoader/GTASA_CLEO_AudioStreams
    Log::Level(LOG_LEVEL::LOG_BOTH) << "Loading BASS..." << std::endl;
    if (!(BASS = (IBASS*)GetInterface("BASS")))
    {
        Log::Level(LOG_LEVEL::LOG_BOTH) << "BASS was not loaded" << std::endl;
    }
    else {
        Log::Level(LOG_LEVEL::LOG_BOTH) << "BASS loaded: " << BASS << std::endl;
    }

    
    Log::Level(LOG_LEVEL::LOG_BOTH) << "----------------------------" << std::endl;
    Log::Level(LOG_LEVEL::LOG_BOTH) << "Mod: v" << Mod::m_Version << std::endl;
    Log::Level(LOG_LEVEL::LOG_BOTH) << "Menu: v" << Menu::m_Version << std::endl;
    Log::Level(LOG_LEVEL::LOG_BOTH) << "----------------------------" << std::endl;

    //
    
    //void* hGTASA = aml->GetLibHandle("libGTASA.so"); crashes the game
    void* hGTASA = dlopen("libGTASA.so", RTLD_LAZY);
    uintptr_t gameAddr = (uintptr_t)(cleo->GetMainLibraryLoadAddress());

    Log::Level(LOG_LEVEL::LOG_BOTH) << "hGTASA: " << hGTASA << std::endl;

    Log::Level(LOG_LEVEL::LOG_BOTH) << "Getting Syms..." << std::endl;

    SET_TO(m_vecCachedPos, aml->GetSym(hGTASA, "_ZN15CTouchInterface14m_vecCachedPosE"));
    SET_TO(pVehiclePool, aml->GetSym(hGTASA, "_ZN6CPools15ms_pVehiclePoolE"));
    SET_TO(pPedPool, aml->GetSym(hGTASA, "_ZN6CPools11ms_pPedPoolE"));

    SET_TO(ScreenGetWidth, aml->GetSym(hGTASA, "_Z17OS_ScreenGetWidthv"));
    SET_TO(ScreenGetHeight, aml->GetSym(hGTASA, "_Z18OS_ScreenGetHeightv"));
    SET_TO(GetVehicleRef, aml->GetSym(hGTASA, "_ZN6CPools13GetVehicleRefEP8CVehicle"));
    SET_TO(GetVehicleFromRef, aml->GetSym(hGTASA, "_ZN6CPools10GetVehicleEi"));
    SET_TO(GetPedRef, aml->GetSym(hGTASA, "_ZN6CPools9GetPedRefEP4CPed"));
    SET_TO(GetPedFromRef, aml->GetSym(hGTASA, "_ZN6CPools6GetPedEi"));

    SET_TO(camera, cleo->GetMainLibrarySymbol("TheCamera"));
    SET_TO(userPaused, cleo->GetMainLibrarySymbol("_ZN6CTimer11m_UserPauseE"));
    SET_TO(codePaused, cleo->GetMainLibrarySymbol("_ZN6CTimer11m_CodePauseE"));
    
    //
    
    Log::Level(LOG_LEVEL::LOG_BOTH) << "vecCachedPos: x " << m_vecCachedPos->x << ", y " << m_vecCachedPos->y << std::endl;
    Log::Level(LOG_LEVEL::LOG_BOTH) << "pVehiclePool: " << pVehiclePool << std::endl;
    Log::Level(LOG_LEVEL::LOG_BOTH) << "pPedPool: " << pPedPool << std::endl;
    Log::Level(LOG_LEVEL::LOG_BOTH) << "ScreenGetWidth: " << ScreenGetWidth() << std::endl;
    Log::Level(LOG_LEVEL::LOG_BOTH) << "ScreenGetHeight: " << ScreenGetHeight() << std::endl;

    //
    
    Log::Level(LOG_LEVEL::LOG_BOTH) << "Registering opcodes..." << std::endl;

    __reg_op_func(PROCESS_LIB, PROCESS_LIB);
    __reg_op_func(GET_DRAW_ITEM_INFO, GET_DRAW_ITEM_INFO);
    __reg_op_func(SEND_TOUCH_STATE, SEND_TOUCH_STATE);
    __reg_op_func(ADD_LOG_MESSAGE, ADD_LOG_MESSAGE);

    __reg_op_func(MENU_ADD_WINDOW, MENU_ADD_WINDOW);
    __reg_op_func(MENU_REMOVE_WINDOW, MENU_REMOVE_WINDOW);
    __reg_op_func(MENU_ADD_BUTTON, MENU_ADD_BUTTON);
    __reg_op_func(MENU_IS_BUTTON_JUST_PRESSED, MENU_IS_BUTTON_JUST_PRESSED);
    
    ModConfig::DefineVersions();
    ModConfig::ProcessVersionChanges_PreConfigLoad();
    ModConfig::Load();
    ModConfig::ProcessVersionChanges_PostConfigLoad();
    ModConfig::Save();

    Mod::Init();
}
