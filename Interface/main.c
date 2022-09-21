#include <gtk/gtk.h>
#include <unistd.h>
#include <stdio.h>

GtkWindow *window;
GtkWidget *fixed1;

GtkWidget *upload_button;
GtkWidget *reload_button;

GtkWidget *or_label;
GtkImage *sudoku_image;
GtkEntry *upload_entry;

GtkBuilder *builder;


int load_menu ()
{

		
	//establish contact with xml code used to adjust widget settings
	
	builder = gtk_builder_new_from_file("main.glade");
	window = GTK_WINDOW(gtk_builder_get_object(builder,"window"));
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit),NULL);
	

	gtk_builder_connect_signals(builder, NULL);

	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));

	upload_button = GTK_WIDGET(gtk_builder_get_object(builder, "upload_button"));
	reload_button = GTK_WIDGET(gtk_builder_get_object(builder, "reload_button"));
	gtk_widget_hide(reload_button);


	upload_entry = GTK_ENTRY(gtk_builder_get_object(builder, "upload_entry"));

	or_label= GTK_WIDGET(gtk_builder_get_object(builder, "or_label"));


	gtk_widget_show(GTK_WIDGET (window));
	gtk_window_fullscreen(window);
	gtk_window_set_title(window, "SUDOKU SOLVER");
		
	gtk_main();


	return EXIT_SUCCESS;

}

int main (int argc, char *argv[])
{
	gtk_init(&argc, &argv); //init Gtk
	load_menu();

	return EXIT_SUCCESS;

}

void display_image(const char *filename, int show)
{//diplay filename image and deleting all widgets in argument

	if (show)
	{
		sudoku_image = GTK_IMAGE(gtk_builder_get_object(builder, "sudoku_image"));
	
		gtk_image_set_from_file(GTK_IMAGE(sudoku_image),filename);

		gtk_widget_show(reload_button);
	}
	else
	{
		gtk_image_set_from_file(sudoku_image, "");

	}
	
	return;

}

void widget_displayer(GtkWidget **widgets, size_t widget_number, int show)
{
	if (show == 1)
	{
		for(size_t i = 0; i < widget_number; i++)
			gtk_widget_show(GTK_WIDGET(widgets[i]));
	}
	else
	{
		for(size_t i = 0; i < widget_number; i++)
			gtk_widget_hide(GTK_WIDGET(widgets[i]));

	}
	return;
}



void on_upload_button_clicked()
{
	GtkWidget *dialog;
	gint res;
	dialog = gtk_file_chooser_dialog_new ("Open File",
                                      GTK_WINDOW (window),
                                      GTK_FILE_CHOOSER_ACTION_OPEN,
                                      ("Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("OK"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

	res = gtk_dialog_run (GTK_DIALOG (dialog));

	if (res == GTK_RESPONSE_ACCEPT)
  	{
    		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    		const char *filename = gtk_file_chooser_get_filename (chooser);	

		GtkWidget* to_destroy[10] =  {or_label,GTK_WIDGET(upload_button),GTK_WIDGET(upload_entry)};

		display_image(filename,1);
		widget_displayer(to_destroy, 3, -1);

  	}

	gtk_widget_destroy(dialog);
}

//gtk entry
void on_upload_entry_activate()
{
	const char* filename = gtk_entry_get_text(upload_entry); 
	if (access(filename,F_OK) == 0)
  	{
		GtkWidget* to_destroy[50] =  {or_label,GTK_WIDGET(upload_button),GTK_WIDGET(upload_entry)};

		display_image(filename,1);

		widget_displayer(to_destroy, 3, 0);

		
  	}
}

void on_reload_button_clicked()
{
	
	GtkWidget* to_revive[10] =  {or_label,GTK_WIDGET(upload_button),GTK_WIDGET(upload_entry)};
	widget_displayer(to_revive,3,1);

	GtkWidget* to_destroy[10] =  {GTK_WIDGET(reload_button)};
	widget_displayer(to_destroy, 1, 0);

	display_image("",0);
}
