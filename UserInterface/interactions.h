#pragma once

#include "ui.h"
#include "imageGestion.h"
#include "widgetGestion.h"

#define SENSI 1
//come from a library but I was unable to import it
void asprintf(char **strp, const char *fmt, ...);

void on_upload_button_clicked(GtkWidget *widget, gpointer data);

void on_upload_entry_activate(GtkWidget *widget, gpointer data);

void on_back_to_menu_button_clicked(GtkWidget *widget, gpointer data);

void on_save_clicked(GtkWidget *widget, gpointer data);

void resetFilters(Menu *menu);

void on_resetFilters_clicked(GtkWidget *widget, gpointer data);

void on_autoDetect_clicked(GtkWidget *widget, gpointer data);

void on_solve_clicked(GtkWidget *widget, gpointer data);

void on_grayscale_toggled(GtkWidget *widget, gpointer data);

gboolean on_crop_corners_move(GtkWidget *widget, GdkEvent *event, gpointer data);

void on_rotate_left_clicked(GtkWidget *widget, gpointer data);

void on_rotate_right_clicked(GtkWidget *widget, gpointer data);

void on_manuDetect_clicked(GtkWidget *widget, gpointer data);

void leave_manual_crop(Menu *menu);

gboolean on_button_press_event(GtkWidget *w, GdkEventButton *event, gpointer data);

gboolean on_motion_notify_event(GtkWidget *widget, GdkEventMotion *event, gpointer data);
