#include "widgetGestion.h"

void widgetCleanup(GtkWidget ** to_hide, size_t to_hide_number, GtkWidget ** to_show, size_t to_show_number)
{
	widgetHider(to_hide, to_hide_number);
	widgetDisplayer(to_show, to_show_number);
	return;
}

void widgetDisplayer(GtkWidget **widgets, size_t widget_number)
{
	for(size_t i = 0; i < widget_number; i++)
		gtk_widget_show(GTK_WIDGET(widgets[i]));
	return;
}

void widgetHider(GtkWidget **widgets, size_t widget_number)
{
	for(size_t i = 0; i < widget_number; i++)
			gtk_widget_hide(GTK_WIDGET(widgets[i]));
	return;
}