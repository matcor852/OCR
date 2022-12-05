#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "ImageProcessing.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

typedef struct
{
    GtkBuilder *builder;
    GtkWindow *window;
    GtkWidget *fixed1;
    GtkBox *box;
    GtkWidget *back_to_menu;
    GtkWidget *file_select_grid;
    GtkWidget *sudoku_image;
    
    GtkButton *grayscale_button;
    GtkButton *gaussian_button;
    GtkButton *sobel_button;

    GtkButton *autoDetect_button;
    GtkButton *solve_button;
    GtkButton *save_button;

    GtkWidget *filters_grid;

    Image *originImage;
    char *originPath;

    GtkLabel *upload_warn_label;
    GtkLabel *filters_warn_label;
} Menu;

int rmDir(const char *dir);

void initUserInterface();


