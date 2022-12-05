#include "imageGestion.h"


unsigned int cpt = 0;

void newSudokuImage(Menu *menu, char *filename)
{
	Image *image = openImage(filename, 4);
	autoResize(image, WINDOW_WIDTH*0.6, WINDOW_HEIGHT*0.6);
	SudokuImageFromImage(menu, image);
	menu->originPath = filename;
	menu->originImage = image;
}

void SudokuImageFromImage(Menu *menu, Image *image)
{
	if (menu->sudoku_image != NULL)
		gtk_widget_destroy(menu->sudoku_image);
	
	char destname[100];
	tmpSaveImage(image, destname);

	menu->sudoku_image = gtk_image_new_from_file(destname);
	gtk_widget_show(menu->sudoku_image);
	gtk_container_add (GTK_CONTAINER (menu->fixed1), menu->sudoku_image);
	gtk_fixed_move (GTK_FIXED(menu->fixed1), menu->sudoku_image, (WINDOW_WIDTH - 245 - image->width) / 2, (WINDOW_HEIGHT - 450) / 2);
}

void destroySudokuImage(Menu *menu)
{
	if (menu->sudoku_image != NULL)
	{
		gtk_widget_destroy(GTK_WIDGET(menu->sudoku_image));
		menu->sudoku_image = NULL;
		menu->originPath = "";
		freeImage(menu->originImage);
	}
	return;
}

Image *actualImage(Menu *menu)
{
	Image *toPrint = copyImage(menu->originImage);
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(menu->gaussian_button)))
	{
		gaussianBlur(toPrint);
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(menu->sobel_button)))
	{
		sobelFilter(toPrint);
	}
	return toPrint;
}

void refreshImage(GtkWidget *widget, gpointer data)
{
	gtk_widget_show(widget);
	// avoid warning about unused parameter
	Menu *menu = (Menu *)data;
	Image *toPrint = actualImage(menu);
	SudokuImageFromImage(menu, toPrint);
	freeImage(toPrint);
}

void tmpSaveImage(Image *image, char *destname)
{
	SDL_Surface *surface = imageToSurface(image);
	struct stat st_ = {0};
	if (stat("tmp/", &st_) == -1)
		mkdir("tmp/", 0700);
	sprintf(destname, "tmp/%d.png", cpt++);
	if (IMG_SavePNG(surface, destname) != 0)
		errx(1, "Error while saving temp image");
	SDL_FreeSurface(surface);
	return;
}