#pragma once

#include <gtk/gtk.h>

void widgetDisplayer(GtkWidget **widgets, size_t widget_number);
void widgetHider(GtkWidget **widgets, size_t widget_number);
void widgetCleanup(GtkWidget ** to_hide, size_t to_hide_number, GtkWidget ** to_show, size_t to_show_number);