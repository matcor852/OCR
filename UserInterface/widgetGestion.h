#pragma once

#include "ui.h"
#include <gtk/gtk.h>

void widgetDisplayer(GtkWidget **widgets);
void widgetHider(GtkWidget **widgets);
void widgetCleanup(GtkWidget ** to_hide, GtkWidget ** to_show);
void displayWarning(GtkLabel *label, char * message);
void changeSensivityWidgets(GtkWidget **widget, int true);

void centerWidget(GtkFixed *fixed, GtkWidget *widget);