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