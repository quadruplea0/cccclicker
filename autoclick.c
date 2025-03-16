#include <cassette/cgui-button.h>
#include <cassette/cgui-cell.h>
#include <cassette/cgui-grid.h>
#include <cassette/cgui.h>
#include <cassette/ccfg.h>
#include <cassette/cobj.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/extensions/XTest.h>
#include <stdio.h>
#include <string.h>
#include "main.h"

#include <unistd.h>  // For usleep


void click() {
    printf("Attempting to click...\n");

    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Failed to open X11 display.\n");
        return;
    }

    printf("X11 display opened successfully.\n");

    // Simulate mouse button press
    XTestFakeButtonEvent(display, Button1, True, 0);
    XFlush(display);
    printf("Mouse button pressed.\n");

    // Small delay to simulate a real click
    usleep(10000);

    // Simulate mouse button release
    XTestFakeButtonEvent(display, Button1, False, 0);
    XFlush(display);
    printf("Mouse button released.\n");

    XCloseDisplay(display);
    printf("X11 display closed.\n");
}



