#include "interactions.h"

gint slider_handler_id;

void on_upload_button_clicked(GtkWidget *widget, gpointer data)
{
	gtk_widget_show(widget);
	//AVOID WARNING

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
		gtk_window_set_title(menu->window, filename);

		newSudokuImage(menu, filename, "init.png");

		GtkWidget *to_hide[] = {menu->file_select_grid, NULL};
		GtkWidget *to_show[] = {menu->filters_grid, menu->back_to_menu, NULL};
		widgetCleanup(to_hide, to_show);
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
		gtk_window_set_title(menu->window, filename);
		newSudokuImage(menu, filename, "init.png");
		GtkWidget *to_hide[] = {menu->file_select_grid, NULL};
		GtkWidget *to_show[] = {menu->filters_grid, menu->back_to_menu, NULL};
		widgetCleanup(to_hide, to_show);
  	}
	else
	{
		displayWarning(menu->upload_warn_label, "File not found");
	}
	return;
}

void on_back_to_menu_button_clicked(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	gtk_window_set_title(menu->window, "OCR Project");
	GtkWidget* to_revive[] =  {menu->file_select_grid, NULL};
	GtkWidget* to_destroy[] =  {menu->sudoku_image, widget, menu->filters_grid, NULL};
	widgetCleanup(to_destroy, to_revive);
	resetFilters(menu);
	destroySudokuImage(menu);

	return;
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
	Menu *menu = (Menu *)data;
	resetFilters(menu);
	Image *toExtract = openImage(menu->originPath, 1);
	Image *cp = copyImage(toExtract);
	autoResize(toExtract, WINDOW_WIDTH*0.6, WINDOW_HEIGHT*0.6);
	initTrig();
	gaussianBlur(toExtract);
	sobelFilter(toExtract);
	thresholdToUpper(toExtract, 16);
	Quad *quad = detectGrid(toExtract);
	if (quad == NULL)
	{
		displayWarning(menu->filters_warn_label, "No sudoku found");
		freeImage(toExtract);
		freeImage(cp);
		return;
	}
	else
	{
		Image *extracted = extractGrid(cp, quad, toExtract->width, toExtract->height);
		char *path;
		asprintf(&path, "%d%s", rand() % 20, "extracted.png");
		SudokuImageFromImage(menu, extracted, path);
		freeQuad(quad);
		freeImage(extracted);
		freeImage(toExtract);
		freeImage(cp);
		GtkWidget *toNoSens[] = {widget, NULL};
		changeSensivityWidgets(toNoSens, 0);
	}
}

void resetFilters(Menu *menu)
{
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(menu->grayscale_button), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(menu->gaussian_button), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(menu->sobel_button), FALSE);
	GtkWidget *toSens[] = {(GtkWidget*)menu->autoDetect_button, 
							(GtkWidget*)menu->grayscale_button,
							(GtkWidget*)menu->gaussian_button,
							(GtkWidget*)menu->sobel_button, NULL};
	changeSensivityWidgets(toSens, 1);
}

void on_resetFilters_clicked(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	resetFilters(menu);
	GtkWidget *toSens[] = {(GtkWidget*)menu->autoDetect_button, 
							(GtkWidget*)menu->grayscale_button,
							(GtkWidget*)menu->gaussian_button,
							(GtkWidget*)menu->sobel_button, NULL};
	changeSensivityWidgets(toSens, 1);
	refreshImage(widget, data);
}

void on_solve_clicked(GtkWidget *widget, gpointer data)
{
	gtk_widget_show(widget);
	Menu *menu = (Menu *)data;
	printf("TODO solve %p\n", menu);
}

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

/*
void connect_slider_handler(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	slider_handler_id = g_signal_connect(G_OBJECT(widget), "value-changed", G_CALLBACK(on_angle_slider_value_changed), menu);
}

void on_angle_slider_value_changed(GtkWidget *widget, gpointer data)
{
	//Menu *menu = (Menu *)data;
	//disconnect signal while already rotating the image to avoid spamming
	g_signal_handler_disconnect(G_OBJECT(widget), slider_handler_id);
	printf("TODOangle%d\n", (int)(gtk_range_get_value(GTK_RANGE(widget)) * 400));
	connect_slider_handler(widget, data);
}
*/