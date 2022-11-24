#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

typedef struct
{
    GtkWindow *window;
    GtkWidget *fixed1;
    GtkBox *box;
    GtkWidget *back_to_menu;
    GtkWidget *upload_button;
    GtkWidget *upload_entry;
    GtkWidget *sudoku_image;
} Menu;

void destroyMenu(Menu *menu);

int startMenu();