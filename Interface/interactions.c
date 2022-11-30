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

		showImage(menu, filename, TRUE);

		GtkWidget *to_hide[2] = {widget, menu->upload_entry};
		widgetHider(to_hide, 2);
		gtk_widget_show(GTK_WIDGET(menu->buttons));
  	}
	gtk_widget_destroy(dialog);
}

//gtk entry
void on_upload_entry_activate(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	const char* filename = gtk_entry_get_text(GTK_ENTRY(widget));
	char path = *filename;
	if (access(filename,F_OK) == 0)
  	{
		GtkWidget *to_hide[2] = {menu->upload_button, widget};
		widgetHider(to_hide, 2);
		gtk_widget_show(GTK_WIDGET(menu->buttons));
		showImage(menu, &path, TRUE);
  	}
	else
	{
		char * message;
		asprintf(&message, "%s not found :/", filename);
		gtk_entry_set_text(GTK_ENTRY(widget), message);
		
	}
	return;
}

void on_back_to_menu_button_clicked(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	GtkWidget* to_revive[2] =  {menu->upload_button, menu->upload_entry};
	GtkWidget* to_destroy[3] =  {menu->sudoku_image, widget, (GtkWidget *)menu->buttons};
	widgetCleanup(to_destroy, 3, to_revive, 2);
	showImage(menu, "", FALSE);

	return;
}


void on_grayscale_toggled()
{
	//TODO
}

void on_gaussian_toggled()
{
	//TODO
}

void on_sobel_toggled()
{
	//TODO
}

void on_auto_detect_toggled()
{
	//TODO
}

void on_manual_detect_toggled()
{
	//TODO
}

void on_solve_toggled()
{
	//TODO
}

void on_save_toggled()
{
	//TODO
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