#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include "ui.h"
#include "widgetGestion.h"
#include "interactions.h"

void newSudokuImage(Menu *menu, char *filename);

void SudokuImageFromImage(Menu *menu, Image *image);

void destroySudokuImage(Menu *menu);

Image *actualImage(Menu *menu);

void refreshImage(GtkWidget *widget, gpointer data);

void tmpSaveImage(Image *image, char *destname);

void loadImage(Menu *menu, char *filename);

//char *getPathExtension(char *path);

gboolean isLoadableImage(char *path);