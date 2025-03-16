// keypress.c
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <stdio.h>
#include <pthread.h>
#include "keypress.h"
#include "main.h"
#include <X11/keysym.h>

extern bool running;

void *detect_keypress(void *arg) {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Failed to open X11 display.\n");
        return NULL;
    }

    // grab needed keys
    XGrabKey(display, XKeysymToKeycode(display, XK_F6), 0, DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, XKeysymToKeycode(display, XK_F7), 0, DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, XKeysymToKeycode(display, XK_F8), 0, DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);

    while (running) {
        XEvent event;
        XNextEvent(display, &event);

        if (event.type == KeyPress) {
            KeySym key = XLookupKeysym(&event.xkey, 0);
            if (key == XK_F6) { // start autoclicker when F6 pressed
                start_auto_click(NULL);
            } else if (key == XK_F7) { // stop autoclicker when F7 pressed
                stop_auto_click(NULL);
            } else if (key == XK_F8) { // quit when F8 pressed
                running = false;
                break; // exit loop
            }
        }
    }

    // ungrab keys
    XUngrabKey(display, XKeysymToKeycode(display, XK_F6), 0, DefaultRootWindow(display));
    XUngrabKey(display, XKeysymToKeycode(display, XK_F7), 0, DefaultRootWindow(display));
    XUngrabKey(display, XKeysymToKeycode(display, XK_F8), 0, DefaultRootWindow(display));

    XCloseDisplay(display);
    return NULL;
}