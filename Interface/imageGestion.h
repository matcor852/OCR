#pragma once

#include <gtk/gtk.h>
#include "menu.h"
#include "widgetGestion.h"
#include "interactions.h"


void showImage(Menu *menu, char *filename, gboolean show);

void reloadImage(Menu *menu, char *path);