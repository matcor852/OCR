#include "imageGestion.h"


void newSudokuImage(Menu *menu, char *filename, char *destname)
{
	if (menu->sudoku_image != NULL)
		gtk_widget_destroy(GTK_WIDGET(menu->sudoku_image));
	Image *image = openImage(filename, 4);
	autoResize(image, WINDOW_WIDTH*0.6, WINDOW_HEIGHT*0.6);
	char * s;
	asprintf(&s, "tmp/%s",destname);

	tmpSaveImage(image, s);
	menu->sudoku_image = gtk_image_new_from_file(s);
	gtk_widget_show(menu->sudoku_image);
	gtk_container_add (GTK_CONTAINER (menu->fixed1), menu->sudoku_image);
	gtk_fixed_move (GTK_FIXED(menu->fixed1), menu->sudoku_image, (WINDOW_WIDTH - 245 - image->width) / 2, (WINDOW_HEIGHT - 500 + 50) / 2);
	menu->actualPath = s;
	menu->originPath = s;
	freeImage(image);
}

void SudokuImageFromImage(Menu *menu, Image *image, char *destname)
{
	autoResize(image, WINDOW_WIDTH*0.6, WINDOW_HEIGHT*0.6);
	char * s;
	asprintf(&s, "tmp/%s",destname);
	tmpSaveImage(image, s);
	if (menu->sudoku_image != NULL)
		gtk_widget_destroy(GTK_WIDGET(menu->sudoku_image));
	
	menu->sudoku_image = gtk_image_new_from_file(s);
	gtk_widget_show(menu->sudoku_image);

	gtk_container_add (GTK_CONTAINER (menu->fixed1), menu->sudoku_image);
	gtk_fixed_move (GTK_FIXED(menu->fixed1), menu->sudoku_image, (WINDOW_WIDTH - 245 - image->width) / 2, (WINDOW_HEIGHT - 500 + 50) / 2);
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
	char *location = menu->actualPath;
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
	asprintf(&imName, "gr%dga%ds%d.png", gr, ga, s);
	SudokuImageFromImage(menu, toPrint, imName);
	freeImage(toPrint);

}

void tmpSaveImage(Image *image, char *destname)
{
	SDL_Surface *surface = imageToSurface(image);
	char path[40];
	struct stat st_ = {0};
	if (stat("tmp/", &st_) == -1)
		mkdir("tmp/", 0700);
	sprintf(path, "%s", destname);
	if (IMG_SavePNG(surface, path) != 0)
		errx(1, "Error while saving temp image");
	SDL_FreeSurface(surface);
	return;
}