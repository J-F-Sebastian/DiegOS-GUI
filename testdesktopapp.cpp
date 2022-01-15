#include <iostream>

#include "desktopapp.h"

int main(int argc, char *argv[])
{
    DesktopApp myApp;
    Rectangle win(10, 10, 700, 400);

    myApp.createWindow(win, "My Window");
    win.width(500);
    win.move(25, 45);
    myApp.createWindow(win, "My other Window");
    win.width(400);
    win.height(200);
    win.move(150, 125);
    myApp.createWindow(win, "Next");
    myApp.run();
}