#include <gtk/gtk.h>
/*#include <unistd.h>
#include <string.h>
#include <math.h>
#include <ctype.h>*/

GtkWindow *window;

GtkWidget *fixed1;
GtkWidget *button1;

GtkImage *image1;

GtkBuilder *builder;

int main (int argc, char *argv[])
{

	gtk_init(&argc, &argv); //init Gtk
	
	//establish contact with xml code used to adjust widget settings
	
	builder = gtk_builder_new_from_file("main.glade");

	window = GTK_WINDOW(gtk_builder_get_object(builder,"window"));

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit),NULL);
	
	gtk_builder_connect_signals(builder, NULL);

	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));

	button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));

	image1 = GTK_IMAGE(gtk_builder_get_object(builder, "image1"));

	entry1 = GTK_ENTRY(gtk_builder_get_object(builder, "entry1"));



	gtk_widget_show(GTK_WIDGET (window));
	
	gtk_window_fullscreen(window);
	gtk_window_set_title(window, "SUDOKU SOLVER");
		
	gtk_main();


	return EXIT_SUCCESS;

}

void on_button1_clicked()
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
    		char *filename;
    		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    		filename = gtk_file_chooser_get_filename (chooser);

		gtk_widget_destroy(button1);
                gtk_widget_destroy(entry1);


		gtk_image_set_from_file(image1,filename);

  	}
	gtk_widget_destroy(dialog);
}

//gtk entry
void on_entry1_past_clipboard()
{
	res = gtk_entry_get_text(); 
	if (res != 0)
  	{

		gtk_widget_destroy(button1);
                gtk_widget_destroy(entry1);
		gtk_image_set_from_file(image1,res);

  	}

}
