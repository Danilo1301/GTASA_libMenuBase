#include "WindowTest.h"

#include "Log.h"

Window* WindowTest::m_Window = NULL;

void WindowTest::Create()
{
	if (m_Window) return;

    auto window = m_Window = Menu::AddWindow(6);
    window->showPageControls = true;

    window->AddCheckbox(29, &Menu::m_DrawCursor);

    auto button_close = window->AddButton(7, CRGBA(170, 70, 70));
    button_close->onClick = []()
    {
        Remove();
    };
}

void WindowTest::Remove()
{
    m_Window->RemoveThisWindow();
    m_Window = NULL;
}