#include <stdint.h>

#include "cleo.h"
#include "cleoaddon.h"

#include <mod/logger.h>

#include "menu/Draw.h"
#include "menu/Menu.h"
#include "menu/CleoMenu.h"

#include "Input.h"

#define __decl_op(__name, __int)	const char* NAME_##__name = #__name; const uint16_t OP_##__name = __int;

#define __handler_params 			void *handle, uint32_t *ip, uint16_t opcode, const char *name

// Size of array
#define sizeofA(__aVar)  ((int)(sizeof(__aVar)/sizeof(__aVar[0])))

#define __reg_opcode				cleo->RegisterOpcode

#define __reg_func					cleo->RegisterOpcodeFunction

#define __op_name_match(x) 			opcode == OP_##x || strcmp(name, NAME_##x) == 0

#define __reg_op_func(x, h) 		__reg_opcode(OP_##x, h); __reg_func(NAME_##x, h);

#define __readParam(handle)         reinterpret_cast<cleo_ifs_t::data_t*>(cleo->ReadParam(handle))
#define __getPointerToScriptVar(handle)         reinterpret_cast<cleo_ifs_t::data_t*>(cleo->GetPointerToScriptVar(handle))

extern cleo_ifs_t* cleo;
extern cleo_addon_ifs_t* cleoaddon;

__decl_op(PROCESS_LIB, 0x0F50); // 0F50=1,process_lib deltaMs %1d%
__decl_op(GET_DRAW_ITEM_INFO, 0x0F51); // 0F51=3,%3g% = get_draw_item_info %1d% id %2d%
__decl_op(SEND_TOUCH_STATE, 0x0F52); //0F52=2,send_touch_state %1d% state %2d%
__decl_op(ADD_LOG_MESSAGE, 0x0F53); //0F53=1,add_log_message %1d%
__decl_op(MENU_ADD_WINDOW, 0x0F54); //0F54=5,%5d% = menu_add_window gxt_id %1d% x %2f% y %3f% width %4f%
__decl_op(MENU_REMOVE_WINDOW, 0x0F55); //0F55=1,menu_remove_window %1d%
__decl_op(MENU_ADD_BUTTON, 0x0F56); //0F56=5,%5d% = menu_add_button gxt_id %1d% to_window %2d% num1 %3d% num2 %4d%
__decl_op(MENU_IS_BUTTON_JUST_PRESSED, 0x0F57); //0F57=2,%2d% = menu_is_button_just_pressed %1d%

static void PROCESS_LIB(__handler_params)
{
    int dt = __readParam(handle)->i;
    
    //Log::file << "PROCESS_LIB dt=" << dt << std::endl;
    //logger->Info("PROCESS_LIB dt=%d", dt);

    Mod::Update(dt);
}

static void GET_DRAW_ITEM_INFO(__handler_params)
{
    eDrawInfoType type = (eDrawInfoType)__readParam(handle)->i;
    int id = __readParam(handle)->i;
    auto result = __getPointerToScriptVar(handle);

    char szTemp[256];
    sprintf(szTemp, "GET_DRAW_ITEM_INFO type=%d, id=%d", type, id);
    //Log::file << szTemp << std::endl;

    if (type == eDrawInfoType::AMOUNT_OF_DRAWITEMS)
    {
        result->i = Draw::m_DrawItems.size();
        return;
    }

    if (type == eDrawInfoType::MENU_OFFSET_X)
    {
        //Log::file << "MENU_OFFSET_X " << Menu::m_MenuOffset.x << std::endl;
        result->f = Menu::m_MenuOffset.x;
        return;
    }

    auto item = Draw::m_DrawItems[id];

    if (type == eDrawInfoType::TYPE)
    {
        result->i = (int)item->type;
        return;
    }

    if (type == eDrawInfoType::POS_X)
    {
        result->f = item->pos.x + item->size.x / 2.0f; //add, so it draw centered
        return;
    }

    if (type == eDrawInfoType::POS_Y)
    {
        result->f = item->pos.y + item->size.y / 2.0f; //add, so it draw centered
        return;
    }

    if (type == eDrawInfoType::SIZE_X) 
    {
        result->f = item->size.x;
        return;
    }

    if (type == eDrawInfoType::SIZE_Y)
    {
        result->f = item->size.y;
        return;
    }

    if (type == eDrawInfoType::COLOR_R) 
    {
        result->i = item->color.r;
        return;
    }

    if (type == eDrawInfoType::COLOR_G)
    {
        result->i = item->color.g;
        return;
    }

    if (type == eDrawInfoType::COLOR_B)
    {
        result->i = item->color.b;
        return;
    }

    if (type == eDrawInfoType::COLOR_A)
    {
        result->i = item->color.a;
        return;
    }

    if (type == eDrawInfoType::GXT_ID)
    {
        result->i = item->gxtId;
        return;
    }

    if (type == eDrawInfoType::NUM_1)
    {
        result->i = item->num1;
        return;
    }

    if (type == eDrawInfoType::NUM_2)
    {
        result->i = item->num2;
    }

    if (type == eDrawInfoType::ALIGN)
    {
        result->i = (int)item->textAlign;
        return;
    }

    if (type == eDrawInfoType::SPRITE_ID)
    {
        result->i = item->spriteId;
        return;
    }
}

static void SEND_TOUCH_STATE(__handler_params)
{
    int touchId = __readParam(handle)->i;
    int state = __readParam(handle)->i;

    char szTemp[256];
    sprintf(szTemp, "SEND_TOUCH_STATE touchId=%d state=%d", touchId, state);
    //Log::file << szTemp << std::endl;

    Input::SetTouchState(touchId, state == 1);
}

static void ADD_LOG_MESSAGE(__handler_params)
{
    int num = __readParam(handle)->i;
    
    Log::Level(LOG_LEVEL::LOG_UPDATE) << "Log: CLEO " << num << std::endl;
}

static void MENU_ADD_WINDOW(__handler_params)
{
    int gxtId = __readParam(handle)->i;
    float x = __readParam(handle)->f;
    float y = __readParam(handle)->f;
    float width = __readParam(handle)->f;
    auto result = __getPointerToScriptVar(handle);

    result->i = CleoMenu::AddWindow(gxtId, x, y, width);
}

static void MENU_REMOVE_WINDOW(__handler_params)
{
    int windowId = __readParam(handle)->i;

    Log::Level(LOG_LEVEL::LOG_BOTH) << "MENU_REMOVE_WINDOW windowId=" << windowId << std::endl;

    CleoMenu::RemoveWindow(windowId);
}

static void MENU_ADD_BUTTON(__handler_params)
{
    int gxtId = __readParam(handle)->i;
    int windowId = __readParam(handle)->i;
    int num1 = __readParam(handle)->i;
    int num2 = __readParam(handle)->i;
    auto result = __getPointerToScriptVar(handle);

    Log::Level(LOG_LEVEL::LOG_BOTH) << "MENU_ADD_BUTTON windowId=" << windowId << std::endl;

    result->i = CleoMenu::AddButton(gxtId, windowId, num1, num2);
}

static void MENU_IS_BUTTON_JUST_PRESSED(__handler_params)
{
    int buttonId = __readParam(handle)->i;
    auto result = __getPointerToScriptVar(handle);

    Log::Level(LOG_LEVEL::LOG_BOTH) << "MENU_IS_BUTTON_JUST_PRESSED buttonId=" << buttonId << std::endl;

    result->i = CleoMenu::HasButtonJustBeenPressed(buttonId) ? 1 : 0;
    //cleoaddon->UpdateCompareFlag(handle, CleoMenu::HasButtonJustBeenPressed(buttonId));
}