#include "imageGestion.h"

void showImage(Menu *menu, char *filename, gboolean show)
{
	if (show)
	{
		// Load the image
		GdkPixbuf * pixbuf = gdk_pixbuf_new_from_file_at_size(filename, 500, 500, NULL);
		int imWidth = gdk_pixbuf_get_width(pixbuf);
		menu->sudoku_image = gtk_image_new_from_pixbuf(pixbuf);
		menu->imPath = filename;
		// Place the image in the window
		gtk_container_add (GTK_CONTAINER (menu->fixed1), menu->sudoku_image);
		gtk_fixed_move (GTK_FIXED(menu->fixed1), menu->sudoku_image, (WINDOW_WIDTH - imWidth) / 2, (WINDOW_HEIGHT - 500 + 50) / 2);

		/* Other part of 4 points dragging shit
		WidgetMover *wgMover = newWidgetMover(GTK_FIXED(menu->fixed1), GTK_WINDOW(menu->window));
		GtkButton *radio = GTK_BUTTON(gtk_builder_get_object(menu->builder, "radio"));
		gtk_widget_show(GTK_WIDGET(radio));
		

		g_signal_connect(radio, "button-press-event", G_CALLBACK(on_button_press_event), wgMover);
		g_signal_connect(radio, "motion-notify-event", G_CALLBACK(on_motion_notify_event), wgMover);
		*/
		// Show the image and back to menu button
		GtkWidget *toShow[2] = {menu->sudoku_image, menu->back_to_menu};
		widgetDisplayer(toShow, 2);
	}
	else
	{
		if (menu->sudoku_image != NULL)
		{
			// Hide the image
			gtk_widget_destroy(GTK_WIDGET(menu->sudoku_image));
			menu->sudoku_image = NULL;
			menu->imPath = "";
		}
	}
	return;
}

void reloadImage(Menu *menu, char *path)
{
	if(menu->sudoku_image)
	{
		gtk_container_remove(GTK_CONTAINER(menu->fixed1),menu->sudoku_image);
    }
	GdkPixbuf * pixbuf = gdk_pixbuf_new_from_file_at_size(path, 500, 500, NULL);
	int imWidth = gdk_pixbuf_get_width(pixbuf);
	menu->sudoku_image = gtk_image_new_from_pixbuf(pixbuf);
	menu->imPath = path;

	// Place the image in the window
	gtk_container_add (GTK_CONTAINER (menu->fixed1), menu->sudoku_image);
	gtk_fixed_move (GTK_FIXED(menu->fixed1), menu->sudoku_image, (WINDOW_WIDTH - imWidth) / 2, (WINDOW_HEIGHT - 500 + 50) / 2);

    gtk_widget_show(menu->sudoku_image);
}