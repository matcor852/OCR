#pragma once

#include "ui.h"
#include "widgetGestion.h"
#include "interactions.h"

void newSudokuImage(Menu *menu, char *filename, char *destname);
void SudokuImageFromImage(Menu *menu, Image *image, char *destname);

void destroySudokuImage(Menu *menu);

void refreshImage(GtkWidget *widget, gpointer data);
