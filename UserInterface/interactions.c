#include "interactions.h"

void on_upload_button_clicked(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	GtkWidget *dialog;
	gint res;
	dialog = gtk_file_chooser_dialog_new ("Choose image to open",
                                      GTK_WINDOW (menu->window),
                                      GTK_FILE_CHOOSER_ACTION_OPEN,
                                      ("Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("This one"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);
	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
  	{
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
		char *filename = gtk_file_chooser_get_filename (chooser);
		newSudokuImage(menu, filename, "init.png");

		GtkWidget *to_hide[2] = {menu->upload_entry, widget};
		GtkWidget *to_show[2] = {(GtkWidget *)menu->allButtons, menu->back_to_menu};
		widgetCleanup(to_hide, 2, to_show, 2);
  	}
	gtk_widget_destroy(dialog);
}

//gtk entry
void on_upload_entry_activate(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	const char* path = gtk_entry_get_text(GTK_ENTRY(widget));
	char *filename = (char *)path;
	if (access(filename,F_OK) == 0)
  	{
		newSudokuImage(menu, filename, "init.png");

		GtkWidget *to_hide[2] = {menu->upload_button, widget};
		GtkWidget *to_show[2] = {(GtkWidget *)menu->allButtons, menu->back_to_menu};
		widgetCleanup(to_hide, 2, to_show, 2);
  	}
	else
	{
		char * message;
		asprintf(&message, "%s not found :/", path);
		gtk_entry_set_text(GTK_ENTRY(widget), message);
	}
	return;
}

void on_back_to_menu_button_clicked(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	GtkWidget* to_revive[2] =  {menu->upload_button, menu->upload_entry};
	GtkWidget* to_destroy[3] =  {menu->sudoku_image, widget, (GtkWidget *)menu->allButtons};
	widgetCleanup(to_destroy, 3, to_revive, 2);
	set_untoggledFilters(menu);
	destroySudokuImage(menu);

	return;
}

void refreshImage(GtkWidget *widget, gpointer data)
{
	gtk_widget_show(widget);
	// avoid warning about unused parameter
	
	Menu *menu = (Menu *)data;
	char *location = menu->originPath;
	Image *toPrint;
	char *imName;
	int gr = 0;
	int ga = 0;
	int s = 0;
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(menu->grayscale_button))){
		gr = 1;
		toPrint = openImage(location, 1);
	}
	else{toPrint = openImage(location, 4);}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(menu->gaussian_button))){
		ga = 1;
		gaussianBlur(toPrint);
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(menu->sobel_button))){
		s = 1;
		sobelFilter(toPrint);
	}
	int angle = (int)(gtk_range_get_value(GTK_RANGE(menu->angle_slider)) * 400);
	asprintf(&imName, "gr%dga%ds%d%d.png", gr, ga, s, angle);
	/*
	if(angle!=0)
	{
		Image *rotated = rotateImage(toPrint, angle, 0);
		SudokuImageFromImage(menu, rotated, imName);
		freeImage(rotated);
		freeImage(toPrint);
	}
	else
	{ */
		SudokuImageFromImage(menu, toPrint, imName);
		freeImage(toPrint);
	//}
}

void on_angle_slider_value_changed(GtkWidget *widget, gpointer data)
{
	gtk_widget_hide(widget);
	printf("TODOangle%d\n", (int)(gtk_range_get_value(GTK_RANGE(widget)) * 400));
	refreshImage(widget, data);
	gtk_widget_show(widget);
}

void on_save_clicked(GtkWidget *widget, gpointer data)
{
	gtk_widget_show(widget);
	// avoid warning about unused parameter
	Menu *menu = (Menu *)data;
	GtkWidget *dialog;
	GtkFileChooser *chooser;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
	gint res;
	dialog = gtk_file_chooser_dialog_new ("Save File",
										menu->window,
										action,
										("Cancel"),
										GTK_RESPONSE_CANCEL,
										("Save"),
										GTK_RESPONSE_ACCEPT,
										NULL);
	chooser = GTK_FILE_CHOOSER (dialog);

	gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);
	gtk_file_chooser_set_current_name (chooser, "Untitled.png");

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	{
		char *filename;
		filename = gtk_file_chooser_get_filename (chooser);
		char *path = menu->actualPath;
		destroySudokuImage(menu);
		Image * toSave = openImage(path, 4);
		saveImage(toSave, filename);
		freeImage(toSave);
		refreshImage(widget, data);
	}

	gtk_widget_destroy (dialog);
}

void on_autoDetect_clicked(GtkWidget *widget, gpointer data)
{
	gtk_widget_show(widget);
	// avoid warning about unused parameter
	Menu *menu = (Menu *)data;
	printf("Automatic detection\n");
	//TODO: extract grid
}

void set_untoggledFilters(Menu *menu)
{
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(menu->grayscale_button), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(menu->gaussian_button), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(menu->sobel_button), FALSE);
	gtk_range_set_value (GTK_RANGE(menu->angle_slider), 0);
}

void on_resetFilters_clicked(GtkWidget *widget, gpointer data)
{
	gtk_widget_show(widget);
	// avoid warning about unused parameter
	Menu *menu = (Menu *)data;
	set_untoggledFilters(menu);
	refreshImage(widget, data);
}

void on_solve_clicked(GtkWidget *widget, gpointer data)
{
	gtk_widget_show(widget);
	// avoid warning about unused parameter
	Menu *menu = (Menu *)data;
	printf("TODO: solve\n");
}
/*
These 2 function are a try to make 4 points dragable to make a square
of the sudoku grid. It's not working yet.


gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	WidgetMover *mover = (WidgetMover *)data;
	if (event->button == 1)
	{
		if (event->button == 1)
		{
			// offset == distance of parent widget from edge of screen ...
			int winW, winH;
			gtk_window_get_size (mover->window, &winW, &winH);
			printf("winW: %d, winH: %d\n", winW, winH);
			int widgetW, widgetH;
			gtk_widget_get_size_request(widget, &widgetW, &widgetH);
			// plus distance from pointer to edge of widget
			// maxx, maxy both relative to the parent
			// note that we're rounding down now so that these max values don't get
			// rounded upward later and push the widget off the edge of its parent.
			*mover->maxX = winW;
			*mover->maxY = winH;
		}
	}
	return TRUE;
}

gboolean on_motion_notify_event(GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
	// x_root,x_root relative to screen
	// x,y relative to parent (fixed widget)
	// px,py stores previous values of x,y
	WidgetMover *mover = (WidgetMover *)data;
	// get starting values for x,y
	int x = event->x;
	int y = event->y;
	// make sure the potential coordinates x,y:
	//   1) will not push any part of the widget outside of its parent container
	//   2) is a multiple of Sensitivity
	if (x != *mover->posX || y != *mover->posY)
	{
		*mover->posX = x;
		*mover->posY = y;
		gtk_fixed_move(GTK_FIXED(mover->fixed1), widget, x, y);
	}
	return TRUE;
}
*/