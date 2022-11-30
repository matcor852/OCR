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
    GtkWidget *upload_button;
    GtkWidget *upload_entry;
    GtkWidget *sudoku_image;

    
    GtkButton *grayscale_button;
    GtkButton *gaussian_button;
    GtkButton *sobel_button;
    GtkScale *angle_slider;

    GtkButton *autoDetect_button;
    GtkButton *solve_button;
    GtkButton *save_button;

    GtkGrid *allButtons;

    char *actualPath;
    char *originPath;
} Menu;

int rmDir(const char *dir);
void startMenu();