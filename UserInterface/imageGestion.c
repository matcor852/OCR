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
	menu->imageOrigin->x = (WINDOW_WIDTH - 245 - image->width) / 2;
	menu->imageOrigin->y = (WINDOW_HEIGHT - 450) / 2;
}

void destroySudokuImage(Menu *menu)
{
	if (menu->originImage != NULL)
	{
		gtk_widget_destroy(GTK_WIDGET(menu->sudoku_image));
		menu->sudoku_image = NULL;
		freeImage(menu->originImage);
		menu->originImage = NULL;
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
	(void)widget;
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
	if (stat("tmpImg/", &st_) == -1)
		mkdir("tmpImg/", 448);
	sprintf(destname, "tmpImg/%d.png", cpt++);
	if (IMG_SavePNG(surface, destname) != 0)
		errx(1, "Error while saving temp image");
	SDL_FreeSurface(surface);
	return;
}

void loadImage(Menu *menu, char *filename)
{
	if (isLoadableImage(filename) == FALSE)
	{
		displayWarning(menu->upload_warn_label, "This format is not loadable");
		return;
	}
	gtk_window_set_title(menu->window, filename);
	newSudokuImage(menu, filename);
	GtkWidget *to_hide[] = {menu->file_select_grid, NULL};
	GtkWidget *to_show[] = {menu->filters_grid, menu->back_to_menu, NULL};
	widgetCleanup(to_hide, to_show);
}

gboolean isLoadableImage(char *path)
{
	SDL_Surface *test = IMG_Load(path);
	if (test == NULL)
		return FALSE;
	SDL_FreeSurface(test);
	return TRUE;
}

/*
char *getPathExtension(char *path)
{
	char *p = strchr(path, '/');
	while (p != NULL)
	{
		path = p + 1;
		p = strchr(path, '/');
	}
	p = strchr(path, '.');
	return p;
}
*/