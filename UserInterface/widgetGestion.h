#pragma once

#include <gtk/gtk.h>

void widgetDisplayer(GtkWidget **widgets);
void widgetHider(GtkWidget **widgets);
void widgetCleanup(GtkWidget ** to_hide, GtkWidget ** to_show);
void displayWarning(GtkLabel *label, char * message);
void changeSensivityWidgets(GtkWidget **widget, int true);

typedef struct
{
    GtkFixed *fixed1;
    GtkWindow *window;
    int *posX;
    int *posY;
    int *offsetX;
    int *offsetY;
    int *maxX;
    int *maxY;

} WidgetMover;