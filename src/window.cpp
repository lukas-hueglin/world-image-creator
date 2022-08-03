#include "Window.h"
#include <iostream>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    //HDC hdc;
	//PAINTSTRUCT ps;

    switch(uMsg) {
        /*case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            viewport->draw(hdc, hWnd);
            EndPaint(hWnd, &ps);
            std::cout << "Draw\n";
            break;*/
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window() : m_hInstance(GetModuleHandle(nullptr)), m_viewport(new Viewport), leftDown(false) {
    const wchar_t* CLASS_NAME = L"WindowClass";

    WNDCLASS wndClass = {};
    wndClass.lpszClassName = CLASS_NAME;
    wndClass.hInstance = m_hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpfnWndProc = WindowProc;

    RegisterClass(&wndClass);

    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

    unsigned int width = 640;
    unsigned int height = 480;

    RECT rect;
    rect.left = 100;
    rect.top = 100;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

    m_hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"World Image Viewer",
        style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        m_hInstance,
        NULL
    );

    BOOL value = TRUE;
    DwmSetWindowAttribute(m_hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));

    ShowWindow(m_hWnd, SW_SHOW);
}

Window::~Window(){
    const wchar_t* CLASS_NAME = L"WindowClass";

    UnregisterClass(CLASS_NAME, m_hInstance);

    delete m_viewport;
}

bool Window::ProcessMessages() {
    MSG msg = {};

    while(PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
        switch(msg.message){
            case WM_QUIT:
                return false;
            case WM_PAINT:
                HDC hdc;
                PAINTSTRUCT ps;

                hdc = BeginPaint(m_hWnd, &ps);
                m_viewport->draw(hdc, m_hWnd);
                EndPaint(m_hWnd, &ps);
                break;
            case WM_LBUTTONDOWN:
                leftDownEvent(msg.lParam);
                break;
            case WM_LBUTTONUP:
                leftUpEvent(msg.lParam);
                break;
            case WM_MOUSEWHEEL:
                wheelEvent(msg.wParam);
                break;
        }
        
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

void Window::update() {
    InvalidateRect(m_hWnd, NULL, NULL);

    if (leftDown) {
        POINT p;
        if (GetCursorPos(&p)) {
            if(ScreenToClient(m_hWnd, &p)) {
                m_viewport->pan(p.x - mouseDownX, p.y - mouseDownY);
                mouseDownX = p.x;
                mouseDownY = p.y;
            }
        }
    }
}

Viewport* Window::getViewport() {
    return m_viewport;
}

void Window::leftDownEvent(LPARAM lParam) {
    mouseDownX = GET_X_LPARAM(lParam);
    mouseDownY = GET_Y_LPARAM(lParam);
    leftDown = true;
}

void Window::leftUpEvent(LPARAM lParam) {
    leftDown = false;
}

void Window::wheelEvent(WPARAM wParam) {
    int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    float zoom = wheelDelta * 0.01;
    m_viewport->zoom(zoom);
}