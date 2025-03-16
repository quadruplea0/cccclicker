#include <cassette/cgui-button.h>
#include <cassette/cgui-cell.h>
#include <cassette/cgui-grid.h>
#include <cassette/cgui-stripes.h>
#include <cassette/cgui.h>
#include <cassette/ccfg.h>
#include <cassette/cobj.h>

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/extensions/XTest.h>
#include <stdio.h>
#include "main.h"


#include <unistd.h>

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h>
#include <X11/keysymdef.h>
#include <X11/keysym.h>
#include "keypress.h"



bool auto_clicking = false;
unsigned int click_interval = 2000;
bool running = true;

static long count = 0;
static pthread_t thread;



void *auto_click_thread(void *arg) {
    while (auto_clicking) {
        click(); 
        usleep(click_interval * 1000);
    }
    return NULL;
}


void start_auto_click(cgui_cell *c) {
    if (!auto_clicking) {
        auto_clicking = true;
        pthread_t thread;
        pthread_create(&thread, NULL, auto_click_thread, NULL);
    }
}


void stop_auto_click(cgui_cell *c) {
    auto_clicking = false;
}



int main(int argc, char **argv) {

  if (argc > 1) {
        int new_interval = atoi(argv[1]);
        if (new_interval > 0) {
            click_interval = new_interval;
            printf("Click interval set to %d ms\n", click_interval);
        } else {
            printf("Invalid interval. Using default: %d ms\n", click_interval);
        }
    }



    cgui_window *window;
    cgui_grid *grid;
    cgui_cell *label;
    cgui_cell *button_start;
    cgui_cell *button_stop;
    cgui_cell *filler;

    cgui_init(argc, argv);

    window = cgui_window_create();
    grid = cgui_grid_create(3, 2);
    label = cgui_label_create();
    button_start = cgui_button_create();
    button_stop = cgui_button_create();
    filler = cgui_stripes_create();

    cgui_label_set(label, "Auto Clicker");
    cgui_button_set_label(button_start, "Start (F6)");
    cgui_button_set_label(button_stop, "Stop (F7)");

    // Set button click handlers
    cgui_button_on_click(button_start, start_auto_click);
    cgui_button_on_click(button_stop, stop_auto_click);
	
    cgui_grid_resize_col(grid, 0, 17);
	cgui_grid_resize_col(grid, 1, 17);
	cgui_grid_resize_col(grid, 2, 17);


    cgui_grid_assign_cell(grid, label, 0, 0, 3, 1);
    cgui_grid_assign_cell(grid, filler,  0, 1, 1, 1);	
    cgui_grid_assign_cell(grid, button_start, 1, 1, 1, 1);
    cgui_grid_assign_cell(grid, button_stop, 2, 1, 1, 1);
  
    cgui_window_push_grid(window, grid);
    cgui_window_activate(window);
    
    pthread_t keypress_thread;
    pthread_create(&keypress_thread, NULL, detect_keypress, NULL);

    cgui_run();

    running = false;
    pthread_join(keypress_thread, NULL);

    return 0;
}



