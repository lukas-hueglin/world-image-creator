#pragma once

#include <functional>
#include <iostream>

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

struct Stroke {
    std::function<void(Gdiplus::Graphics&, int, int)> strokeFunction;
    Stroke* next;

    Stroke();
    ~Stroke();
    void draw(Gdiplus::Graphics& gf, int width, int height);
};


class Viewport {
    public:
        Viewport();
        ~Viewport();

        void add_stroke(Stroke* stroke);
        void draw(HDC hdc, HWND hWnd);

        void pan(int xMovement, int yMovenent);
        void zoom(float zoom);
    private:
        ULONG_PTR gdiplusToken;

        Stroke* root;

        float translationX;
        float translationY;
        float mouseZoom;
        float scale;
};