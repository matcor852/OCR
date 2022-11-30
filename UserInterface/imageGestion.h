#pragma once

#include <gtk/gtk.h>
#include "menu.h"
#include "widgetGestion.h"
#include "interactions.h"

void newSudokuImage(Menu *menu, char *filename, char *destname);
void SudokuImageFromImage(Menu *menu, Image *image, char *destname);

void destroySudokuImage(Menu *menu);
