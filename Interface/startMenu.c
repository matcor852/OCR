#include "menu.h"
#include "interactions.h"

int startMenu()
{
	//establish contact with glade file
	GtkBuilder *builder = gtk_builder_new_from_file("main.glade");

	GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder,"window"));

	GtkWidget *fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
	GtkBox *box = GTK_BOX(gtk_builder_get_object(builder, "box"));

	GtkWidget *back_to_menu = GTK_WIDGET(gtk_builder_get_object(builder, "back_to_menu"));
	GtkWidget *upload_button = GTK_WIDGET(gtk_builder_get_object(builder, "upload_button"));
	GtkWidget *upload_entry = GTK_WIDGET(gtk_builder_get_object(builder, "upload_entry"));

	GtkWidget *sudoku_image = NULL;
	
	Menu menu;
	menu.window = window;
    menu.fixed1 = fixed1;
    menu.box = box;
    menu.back_to_menu = back_to_menu;
    menu.upload_button = upload_button;
    menu.upload_entry = upload_entry;
    menu.sudoku_image = sudoku_image;
	
	//connect signals
	g_signal_connect(back_to_menu, "clicked", G_CALLBACK(on_back_to_menu_button_clicked), &menu);
	gtk_widget_hide(back_to_menu);
	g_signal_connect(upload_button, "clicked", G_CALLBACK(on_upload_button_clicked), &menu);
	g_signal_connect(upload_entry, "activate", G_CALLBACK(on_upload_entry_activate), &menu);

	//window settings
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit),NULL);
	gtk_widget_show(GTK_WIDGET (window));
	gtk_window_set_title(window, "Optical Sudoku Solver");
	gtk_window_set_position(window, GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	gtk_window_set_resizable(window, FALSE);

	gtk_main();

	return EXIT_SUCCESS;

}

void destroyMenu(Menu *menu)
{
    gdk_window_destroy(GDK_WINDOW(menu->window));
    gtk_widget_destroy(GTK_WIDGET(menu->box));
    gtk_widget_destroy(menu->back_to_menu);
    gtk_widget_destroy(menu->upload_button);
    gtk_widget_destroy(menu->upload_entry);
    if (menu->sudoku_image != NULL)
    {
        gtk_widget_destroy(menu->sudoku_image);
    }
    free(menu);
    return;
}