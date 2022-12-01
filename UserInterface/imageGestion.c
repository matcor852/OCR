#include "imageGestion.h"


void newSudokuImage(Menu *menu, char *filename, char *destname)
{
	if (menu->sudoku_image != NULL)
		gtk_widget_destroy(GTK_WIDGET(menu->sudoku_image));
	Image *image = openImage(filename, 4);
	resizeImage(image, 500, 500);
	char * s;
	asprintf(&s, "tmp/%s",destname);

	tmpSaveImage(image, s);
	menu->sudoku_image = gtk_image_new_from_file(s);
	gtk_widget_show(menu->sudoku_image);
	gtk_container_add (GTK_CONTAINER (menu->fixed1), menu->sudoku_image);
	gtk_fixed_move (GTK_FIXED(menu->fixed1), menu->sudoku_image, (WINDOW_WIDTH - 500) / 3, (WINDOW_HEIGHT - 500 + 50) / 2);
	menu->actualPath = s;
	menu->originPath = s;
	freeImage(image);
}

void SudokuImageFromImage(Menu *menu, Image *image, char *destname)
{
	resizeImage(image, 500, 500);
	char * s;
	asprintf(&s, "tmp/%s",destname);
	tmpSaveImage(image, s);
	if (menu->sudoku_image != NULL)
		gtk_widget_destroy(GTK_WIDGET(menu->sudoku_image));
	
	menu->sudoku_image = gtk_image_new_from_file(s);
	gtk_widget_show(menu->sudoku_image);

	gtk_container_add (GTK_CONTAINER (menu->fixed1), menu->sudoku_image);
	gtk_fixed_move (GTK_FIXED(menu->fixed1), menu->sudoku_image, (WINDOW_WIDTH - 500) / 3, (WINDOW_HEIGHT - 500 + 50) / 2);
	menu->actualPath = s;
}

void destroySudokuImage(Menu *menu)
{
	if (menu->sudoku_image != NULL)
	{
		gtk_widget_destroy(GTK_WIDGET(menu->sudoku_image));
		menu->sudoku_image = NULL;
		menu->actualPath = "";
	}
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