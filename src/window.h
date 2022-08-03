#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <dwmapi.h>

#include "viewport.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
    public:
        Window();
        Window(const Window&) = delete;
        Window& operator =(const Window&) = delete;
        ~Window();

        bool ProcessMessages();
        void update();

        Viewport* getViewport();

        void leftDownEvent(LPARAM lParam);
        void leftUpEvent(LPARAM lParam);
        void wheelEvent(WPARAM wParam);

    private:
        HINSTANCE m_hInstance;
        HWND m_hWnd;

        Viewport* m_viewport;

        int mouseDownX;
        int mouseDownY;
        bool leftDown;
};