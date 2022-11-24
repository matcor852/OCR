#include "imageGestion.h"

void showImage(Menu *menu, const char *filename, gboolean show)
{
	if (show)
	{
		// Load the image
		GdkPixbuf * pixbuf = gdk_pixbuf_new_from_file_at_size(filename, 500, 500, NULL);
		menu->sudoku_image = gtk_image_new_from_pixbuf(pixbuf);

		// Place the image in the window
		gtk_container_add (GTK_CONTAINER (menu->fixed1), menu->sudoku_image);
		gtk_fixed_move (GTK_FIXED(menu->fixed1), menu->sudoku_image, 150, 50);

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
		}
	}
	return;
}