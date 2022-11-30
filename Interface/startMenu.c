#include "menu.h"
#include "interactions.h"

void startMenu()
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

	GtkGrid *buttons = GTK_GRID(gtk_builder_get_object(builder, "button_grid"));

	GtkButton *grayscale_button = GTK_BUTTON(gtk_builder_get_object(builder, "grayscale"));
	GtkButton *gaussian_button = GTK_BUTTON(gtk_builder_get_object(builder, "gaussian"));
	GtkButton *sobel_button = GTK_BUTTON(gtk_builder_get_object(builder, "sobel"));

	GtkButton *autoDetect_button = GTK_BUTTON(gtk_builder_get_object(builder, "auto_detect"));
	GtkButton *save_button = GTK_BUTTON(gtk_builder_get_object(builder, "save"));
	GtkButton *solve_button = GTK_BUTTON(gtk_builder_get_object(builder, "solve"));

	

	char * pathP = malloc(sizeof(char) * 100);
	Menu menu =
    {
		.builder = builder,
        .window = window,
        .fixed1 = fixed1,
        .box = box,
        .back_to_menu = back_to_menu,
        .upload_button = upload_button,
        .upload_entry = upload_entry,
        .sudoku_image = sudoku_image,
		.buttons = buttons,
		.grayscale_button = grayscale_button,
		.gaussian_button = gaussian_button,
		.sobel_button = sobel_button,
		.autoDetect_button = autoDetect_button,
		.save_button = save_button,
		.solve_button = solve_button,
		.imPath = pathP
	};

	//connect signals
	g_signal_connect(back_to_menu, "clicked", G_CALLBACK(on_back_to_menu_button_clicked), &menu);
	gtk_widget_hide(back_to_menu);
	gtk_widget_hide(GTK_WIDGET(buttons));
	g_signal_connect(upload_button, "clicked", G_CALLBACK(on_upload_button_clicked), &menu);
	g_signal_connect(upload_entry, "activate", G_CALLBACK(on_upload_entry_activate), &menu);

	//window settings
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit),NULL);

	gtk_widget_show(GTK_WIDGET (window));
	gtk_window_set_title(window, "Optical Sudoku Solver");
	gtk_window_set_position(window, GTK_WIN_POS_CENTER);
	//gtk_window_set_default_size(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	gtk_window_set_resizable(window, FALSE);

	gtk_main();
	return;

}

/*
gboolean on_window_resize(GtkWidget *widget, GdkRectangle *allocation, gpointer data)
{
    Menu *menu = (Menu *)data;
    int new_width, new_height;
    GdkRectangle workarea = {0};
    gdk_monitor_get_workarea(gdk_display_get_primary_monitor(gdk_display_get_default()), &workarea);
    printf("screenwidth: %d, screenheight: %d\n", workarea.width, workarea.height);
    gtk_window_get_size (widget, &new_width, &new_height);
    printf("window: %p, width: %d, height: %d\n", widget, new_width, new_height);
    return TRUE;
}
*/