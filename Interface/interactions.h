#pragma once

#include <gtk/gtk.h>
#include "menu.h"
#include "imageGestion.h"
#include "widgetGestion.h"

#define SENSI 1
//come from a library but I was unable to import it
void asprintf(char **strp, const char *fmt, ...);

void on_upload_button_clicked(GtkWidget *widget, gpointer data);

void on_upload_entry_activate(GtkWidget *widget, gpointer data);

void on_back_to_menu_button_clicked(GtkWidget *widget, gpointer data);

gboolean on_button_press_event(GtkWidget *w, GdkEventButton *event, gpointer data);

gboolean on_motion_notify_event(GtkWidget *widget, GdkEventMotion *event, gpointer data);