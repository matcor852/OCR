#pragma once

#include <gtk/gtk.h>
#include "menu.h"
#include "imageGestion.h"
#include "widgetGestion.h"

void on_upload_button_clicked(GtkWidget *widget, gpointer data);

void on_upload_entry_activate(GtkWidget *widget, gpointer data);

void on_back_to_menu_button_clicked(GtkWidget *widget, gpointer data);