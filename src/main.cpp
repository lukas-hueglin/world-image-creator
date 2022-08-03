#include <iostream>
#include "window.h"
#include "viewport.h"

std::function<void(Gdiplus::Graphics&, int, int)> drawStroke =
    [](Gdiplus::Graphics& gf, int width, int height) {

    Gdiplus::SolidBrush background(Gdiplus::Color(200, 10, 100));
    gf.FillRectangle(&background, width/3, height/3, width/3, height/3);  
};


int main() {
    std::cout << "Creating Window\n";

    Window* pWindow = new Window();
    Viewport* viewport = pWindow->getViewport();

    Stroke* stroke = new Stroke;
    stroke->strokeFunction = drawStroke;
    viewport->add_stroke(stroke);

    bool running = true;
    while(running) {
        if(!pWindow->ProcessMessages()) {
            std::cout << "Closing Window\n";
            running = false;
        }

        pWindow->update();
        Sleep(10);
    }

    delete pWindow;

    return 0;
}