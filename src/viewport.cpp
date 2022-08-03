#include "viewport.h"

Stroke::Stroke():next(nullptr){}

Stroke::~Stroke() {
    if (next != nullptr) {
        delete next;
    }
}

void Stroke::draw(Gdiplus::Graphics& gf, int width, int height) {
    strokeFunction(gf, width, height);
    if(next != nullptr) {
        next->draw(gf, width, height);
    }
}

Viewport::Viewport() : root(new Stroke), translationX(0), translationY(0), mouseZoom(0), scale(1) {

    // initialize gdi+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);


    // Set Background
    std::function<void(Gdiplus::Graphics&, int, int)> drawStroke =
        [](Gdiplus::Graphics& gf, int width, int height) {

        Gdiplus::SolidBrush background(Gdiplus::Color(1, 27, 59));
        gf.FillRectangle(&background, -width, -height, width*2, height*2);  
    };
    root->strokeFunction = drawStroke;
}

Viewport::~Viewport() {
    Gdiplus::GdiplusShutdown(gdiplusToken);
}

void Viewport::add_stroke(Stroke* stroke) {
    Stroke* lastStroke = root;
    while(lastStroke->next != nullptr) {
        lastStroke = lastStroke->next;
    }
    lastStroke->next = stroke;
}

void Viewport::draw(HDC hdc, HWND hWnd) {
    // get width and height
    RECT rect;
    GetWindowRect(hWnd, &rect);
    const unsigned int width = rect.right - rect.left;
    const unsigned int height = rect.bottom - rect.top;

    XFORM xForm;

    SetGraphicsMode(hdc, GM_ADVANCED);
    //SetMapMode(hdc, MM_LOENGLISH);

    
    xForm.eM11 =  scale; 
    xForm.eM12 =  0.0f; 
    xForm.eM21 =  0.0f; 
    xForm.eM22 =  scale; 
    xForm.eDx  =  translationX * (2 - scale) + width/2 * (1 - scale); 
    xForm.eDy  =  translationY * (2 - scale) + height/2 * (1 - scale);
    SetWorldTransform(hdc, &xForm); 

	Gdiplus::Graphics gf(hdc);

    // Draw
    root->draw(gf, width, height);
}

void Viewport::pan(int xMovement, int yMovement) {
    translationX += xMovement;
    translationY += yMovement;
}

void Viewport::zoom(float zoom) {
    mouseZoom += zoom;
    scale = std::exp(mouseZoom);
}