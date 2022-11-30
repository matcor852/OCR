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
/*
WidgetMover *newWidgetMover(GtkFixed *fixed1, GtkWindow *window)
{
	WidgetMover *widgetMover = malloc(sizeof(WidgetMover));
	widgetMover->fixed1 = fixed1;
	widgetMover->window = window;
	widgetMover->posX = malloc(sizeof(int));
	widgetMover->posY = malloc(sizeof(int));
	widgetMover->offsetX = malloc(sizeof(int));
	widgetMover->offsetY = malloc(sizeof(int));
	widgetMover->maxX = malloc(sizeof(int));
	widgetMover->maxY = malloc(sizeof(int));
	*(widgetMover->offsetX) = 0;
	*(widgetMover->offsetY) = 0;
	*(widgetMover->maxX) = 0;
	*(widgetMover->maxY) = 0;
	return widgetMover;
}

void destroyWidgetMover(WidgetMover *widgetMover)
{
	free(widgetMover->posX);
	free(widgetMover->posY);
	free(widgetMover->offsetX);
	free(widgetMover->offsetY);
	free(widgetMover->maxX);
	free(widgetMover->maxY);
	free(widgetMover);
	return;
}
*/