#include "interactions.h"
#include <string.h>
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

		newSudokuImage(menu, filename);

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
		newSudokuImage(menu, filename);
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
		Image * toSave = actualImage(menu);
		saveImage(toSave, filename);
		freeImage(toSave);
		refreshImage(widget, data);
	}
	gtk_widget_destroy (dialog);
}

void on_autoDetect_clicked(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	toGrey(menu->originImage);
	Image *betterForLines = copyImage(menu->originImage);
	initTrig();
	gaussianBlur(betterForLines);
	sobelFilter(betterForLines);
	thresholdToUpper(betterForLines, 16);
	Quad *quad = detectGrid(betterForLines);
	if (quad == NULL)
	{
		displayWarning(menu->filters_warn_label, "No sudoku found");
		freeImage(betterForLines);
		return;
	}
	else
	{
		Image *extracted = extractGrid(menu->originImage, quad, menu->originImage->width, menu->originImage->height);
		freeImage(menu->originImage);
		menu->originImage = extracted;
		SudokuImageFromImage(menu, extracted);
		freeQuad(quad);
		freeImage(betterForLines);
		GtkWidget *toNoSens[] = {widget, GTK_WIDGET(menu->grayscale_button), NULL};
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(menu->grayscale_button), TRUE);
		changeSensivityWidgets(toNoSens, 0);
	}
	refreshImage(widget, data);
}

void resetFilters(Menu *menu)
{
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(menu->grayscale_button), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(menu->gaussian_button), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(menu->sobel_button), FALSE);
	GtkWidget *toSens[] = {GTK_WIDGET(menu->autoDetect_button), GTK_WIDGET(menu->grayscale_button), NULL};
	changeSensivityWidgets(toSens, 1);
}

void on_grayscale_toggled(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	if (menu->originImage == NULL)
	{
		displayWarning(menu->filters_warn_label, "No image loaded");
		return;
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
	{
		toGrey(menu->originImage);
		refreshImage(widget, data);
	}
	else
	{
		freeImage(menu->originImage);
		menu->originImage = openImage(menu->originPath, 4);
		autoResize(menu->originImage, WINDOW_WIDTH*0.6, WINDOW_HEIGHT*0.6);
	}
}

void on_resetFilters_clicked(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	resetFilters(menu);
	freeImage(menu->originImage);
	menu->originImage = openImage(menu->originPath, 4);
	autoResize(menu->originImage, WINDOW_WIDTH*0.6, WINDOW_HEIGHT*0.6);
	refreshImage(widget, data);
}

void on_solve_clicked(GtkWidget *widget, gpointer data)
{
	gtk_widget_show(widget);
	Menu *menu = (Menu *)data;
	printf("TODO solve %p\n", menu);
}

gboolean on_crop_corner_move(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	Menu *menu = (Menu *)data;

	gint actualX, actualY;
	gtk_widget_translate_coordinates(widget, gtk_widget_get_toplevel(widget), 0, 0, &actualX, &actualY);

	gint mouseX = event->button.x, mouseY = event->button.y;

	gint newX = actualX + mouseX, newY = actualY + mouseY;

	gint imPosX, imPosY;
	gtk_widget_translate_coordinates(GTK_WIDGET(menu->sudoku_image), gtk_widget_get_toplevel(GTK_WIDGET(menu->sudoku_image)), 0, 0, &imPosX, &imPosY);
	gint imWidth = gtk_widget_get_allocated_width(menu->sudoku_image), imHeight = gtk_widget_get_allocated_height(menu->sudoku_image);


	if (newX < imPosX + imWidth && newY > imPosY && newY < imPosY + imHeight)
	{
		gtk_fixed_move(GTK_FIXED(menu->fixed1), widget, newX, newY);
	}

	//printf("x: %d, y: %d\n", actualX + mouseX, actualY + mouseY);
	return TRUE;
}

void on_rotate_left_clicked(GtkWidget *widget, gpointer data)
{
	/*
	Menu *menu = (Menu *)data;
	Image *r = rotateImage(menu->originImage, -90, 0);
	freeImage(menu->originImage);
	menu->originImage = r;
	autoResize(menu->originImage, WINDOW_WIDTH*0.6, WINDOW_HEIGHT*0.6);
	refreshImage(widget, data);
	*/
	printf("TODO rotate left\n");
	refreshImage(widget, data);
}

void on_rotate_right_clicked(GtkWidget *widget, gpointer data)
{
	/*
	Menu *menu = (Menu *)data;
	Image *r = rotateImage(menu->originImage, 90, 0);
	freeImage(menu->originImage);
	menu->originImage = r;
	autoResize(menu->originImage, WINDOW_WIDTH*0.6, WINDOW_HEIGHT*0.6);
	refreshImage(widget, data);
	*/
	printf("TODO rotate right \n");
	refreshImage(widget, data);
}

void on_manuDetect_clicked(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	GtkEventBox *crop_corner1 = GTK_EVENT_BOX(gtk_builder_get_object(menu->builder, "crop_corner1"));
	GtkEventBox *crop_corner2 = GTK_EVENT_BOX(gtk_builder_get_object(menu->builder, "crop_corner2"));
	GtkEventBox *crop_corner3 = GTK_EVENT_BOX(gtk_builder_get_object(menu->builder, "crop_corner3"));
	GtkEventBox *crop_corner4 = GTK_EVENT_BOX(gtk_builder_get_object(menu->builder, "crop_corner4"));
	if (strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "Manual crop") == 0)
	{
		gtk_button_set_label(GTK_BUTTON(widget), "Apply crop");

		gint imPosX, imPosY;
		gtk_widget_translate_coordinates(GTK_WIDGET(menu->sudoku_image), gtk_widget_get_toplevel(GTK_WIDGET(menu->sudoku_image)), 0, 0, &imPosX, &imPosY);
		gint imWidth = gtk_widget_get_allocated_width(menu->sudoku_image), imHeight = gtk_widget_get_allocated_height(menu->sudoku_image);

		gtk_container_remove(GTK_CONTAINER(menu->fixed1), GTK_WIDGET(crop_corner1));
		gtk_container_add(GTK_CONTAINER(menu->fixed1), GTK_WIDGET(crop_corner1));
		gtk_fixed_move(GTK_FIXED(menu->fixed1), GTK_WIDGET(crop_corner1), imPosX, imPosY);
		gtk_widget_show(GTK_WIDGET(crop_corner1));

		gtk_container_remove(GTK_CONTAINER(menu->fixed1), GTK_WIDGET(crop_corner2));
		gtk_container_add(GTK_CONTAINER(menu->fixed1), GTK_WIDGET(crop_corner2));
		gtk_fixed_move(GTK_FIXED(menu->fixed1), GTK_WIDGET(crop_corner2), imPosX + imWidth, imPosY);
		gtk_widget_show(GTK_WIDGET(crop_corner2));

		gtk_container_remove(GTK_CONTAINER(menu->fixed1), GTK_WIDGET(crop_corner3));
		gtk_container_add(GTK_CONTAINER(menu->fixed1), GTK_WIDGET(crop_corner3));
		gtk_fixed_move(GTK_FIXED(menu->fixed1), GTK_WIDGET(crop_corner3), imPosX + imWidth, imPosY + imHeight);
		gtk_widget_show(GTK_WIDGET(crop_corner3));

		gtk_container_remove(GTK_CONTAINER(menu->fixed1), GTK_WIDGET(crop_corner4));
		gtk_container_add(GTK_CONTAINER(menu->fixed1), GTK_WIDGET(crop_corner4));
		gtk_fixed_move(GTK_FIXED(menu->fixed1), GTK_WIDGET(crop_corner4), imPosX, imPosY + imHeight);
		gtk_widget_show(GTK_WIDGET(crop_corner4));
	}
	else
	{
		gtk_button_set_label(GTK_BUTTON(widget), "Manual crop");
		Point *p1, *p2, *p3, *p4;
		gint orgX, orgY;
		gtk_widget_translate_coordinates(GTK_WIDGET(menu->sudoku_image), gtk_widget_get_toplevel(GTK_WIDGET(menu->sudoku_image)), 0, 0, &orgX, &orgY);
		gint x, y;
		gtk_widget_translate_coordinates(GTK_WIDGET(crop_corner1), gtk_widget_get_toplevel(GTK_WIDGET(crop_corner1)), 0, 0, &x, &y);
		p1 = newPoint((st)(x - orgX), (st)(y - orgY));
		gtk_widget_translate_coordinates(GTK_WIDGET(crop_corner2), gtk_widget_get_toplevel(GTK_WIDGET(crop_corner2)), 0, 0, &x, &y);
		p2 = newPoint((st)(x - orgX), (st)(y - orgY));
		gtk_widget_translate_coordinates(GTK_WIDGET(crop_corner3), gtk_widget_get_toplevel(GTK_WIDGET(crop_corner3)), 0, 0, &x, &y);
		p3 = newPoint((st)(x - orgX), (st)(y - orgY));
		gtk_widget_translate_coordinates(GTK_WIDGET(crop_corner4), gtk_widget_get_toplevel(GTK_WIDGET(crop_corner4)), 0, 0, &x, &y);
		p4 = newPoint((st)(x - orgX), (st)(y - orgY));
		printf("p1: %zu %zu\n", p1->x, p1->y);
		printf("p2: %zu %zu\n", p2->x, p2->y);
		printf("p3: %zu %zu\n", p3->x, p3->y);
		printf("p4: %zu %zu\n", p4->x, p4->y);
		Quad *quad = newQuad(p1, p2, p4, p3);
		Image *cropped = extractGrid(menu->originImage, quad, menu->originImage->width, menu->originImage->height);
		freeImage(menu->originImage);
		menu->originImage = cropped;
		freeQuad(quad);
		GtkWidget *to_hide[] = {GTK_WIDGET(crop_corner1), GTK_WIDGET(crop_corner2), GTK_WIDGET(crop_corner3), GTK_WIDGET(crop_corner4), NULL};
		widgetHider(to_hide);
		refreshImage(widget, data);
	}
}

void leave_manual_crop(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	GtkEventBox *crop_corner1 = GTK_EVENT_BOX(gtk_builder_get_object(menu->builder, "crop_corner1"));
	GtkEventBox *crop_corner2 = GTK_EVENT_BOX(gtk_builder_get_object(menu->builder, "crop_corner2"));
	GtkEventBox *crop_corner3 = GTK_EVENT_BOX(gtk_builder_get_object(menu->builder, "crop_corner3"));
	GtkEventBox *crop_corner4 = GTK_EVENT_BOX(gtk_builder_get_object(menu->builder, "crop_corner4"));
	printf("leave_manual_crop\n");
	GtkWidget *to_hide[] = {GTK_WIDGET(crop_corner1), GTK_WIDGET(crop_corner2), GTK_WIDGET(crop_corner3), GTK_WIDGET(crop_corner4), NULL};
	widgetHider(to_hide);
}
/*
void on_angle_slider_value_changed(GtkWidget *widget, gpointer data)
{
	//Menu *menu = (Menu *)data;
	//disconnect signal while already rotating the image to avoid spamming
	g_signal_handler_disconnect(G_OBJECT(widget), slider_handler_id);
	printf("TODOangle%d\n", (int)(gtk_range_get_value(GTK_RANGE(widget)) * 400));
	connect_slider_handler(widget, data);
}
*/