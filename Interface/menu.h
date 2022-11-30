#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

typedef struct
{
    GtkBuilder *builder;
    GtkWindow *window;
    GtkWidget *fixed1;
    GtkBox *box;
    GtkWidget *back_to_menu;
    GtkWidget *upload_button;
    GtkWidget *upload_entry;
    GtkWidget *sudoku_image;

    GtkGrid *buttons;
    GtkButton *grayscale_button;
    GtkButton *gaussian_button;
    GtkButton *sobel_button;

    GtkButton *autoDetect_button;
    GtkButton *solve_button;
    GtkButton *save_button;

    char * imPath;
} Menu;


void startMenu();