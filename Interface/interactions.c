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
		const char *filename = gtk_file_chooser_get_filename (chooser);

		showImage(menu, filename, TRUE);

		GtkWidget *to_hide[2] = {widget, menu->upload_entry};
		widgetHider(to_hide, 2);
  	}
	gtk_widget_destroy(dialog);
}

//gtk entry
void on_upload_entry_activate(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	const char* filename = gtk_entry_get_text(GTK_ENTRY(widget)); 
	if (access(filename,F_OK) == 0)
  	{
		GtkWidget *to_hide[2] = {menu->upload_button, widget};
		widgetHider(to_hide, 2);
		showImage(menu, filename, TRUE);
  	}
	else
	{
		gtk_entry_set_text(GTK_ENTRY(widget), "File not found :/");
	}
	return;
}

void on_back_to_menu_button_clicked(GtkWidget *widget, gpointer data)
{
	Menu *menu = (Menu *)data;
	GtkWidget* to_revive[2] =  {menu->upload_button, menu->upload_entry};
	GtkWidget* to_destroy[2] =  {menu->sudoku_image, widget};
	widgetCleanup(to_destroy, 2, to_revive, 2);
	showImage(menu, "", FALSE);
	return;
}