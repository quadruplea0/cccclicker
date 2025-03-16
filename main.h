#ifndef AUTO_CLICKER_H
#define AUTO_CLICKER_H

#include <cassette/cgui.h>

extern bool auto_clicking;
extern unsigned int click_interval;
extern bool running;

void click();
void start_auto_click(cgui_cell *c);
void stop_auto_click(cgui_cell *c);

#endif
