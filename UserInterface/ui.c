#include "ui.h"
#include "interactions.h"
#include <fts.h>

void initUserInterface()
{
	//---------GTK COMPONENTS INITIALIZATION WITH GLADE FILE---------//
	GtkBuilder *builder = gtk_builder_new_from_file("main.glade");
	GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder,"window"));
	GtkWidget *fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
	GtkBox *box = GTK_BOX(gtk_builder_get_object(builder, "box"));
	GtkWidget *file_select_grid = GTK_WIDGET(gtk_builder_get_object(builder, "file_select_grid"));
	GtkWidget *back_to_menu = GTK_WIDGET(gtk_builder_get_object(builder, "back_to_menu"));
	GtkWidget *upload_button = GTK_WIDGET(gtk_builder_get_object(builder, "upload_button"));
	GtkWidget *upload_entry = GTK_WIDGET(gtk_builder_get_object(builder, "upload_entry"));
	
	GtkWidget *filters_grid = GTK_WIDGET(gtk_builder_get_object(builder, "filters_grid"));
	GtkButton *grayscale_button = GTK_BUTTON(gtk_builder_get_object(builder, "grayscale"));
	GtkButton *gaussian_button = GTK_BUTTON(gtk_builder_get_object(builder, "gaussian"));
	GtkButton *sobel_button = GTK_BUTTON(gtk_builder_get_object(builder, "sobel"));
	GtkButton *resetFilters_button = GTK_BUTTON(gtk_builder_get_object(builder, "resetFilters"));
	GtkButton *autoDetect_button = GTK_BUTTON(gtk_builder_get_object(builder, "auto_detect"));
	GtkButton *save_button = GTK_BUTTON(gtk_builder_get_object(builder, "save"));
	GtkButton *solve_button = GTK_BUTTON(gtk_builder_get_object(builder, "solve"));

	GtkButton *manuDetect_button = GTK_BUTTON(gtk_builder_get_object(builder, "manu_detect"));
	GtkButton *rotate_left_button = GTK_BUTTON(gtk_builder_get_object(builder, "rotate_left"));
	GtkButton *rotate_right_button = GTK_BUTTON(gtk_builder_get_object(builder, "rotate_right"));

	GtkLabel *upload_warn_label = GTK_LABEL(gtk_builder_get_object(builder, "upload_warn_label"));
	GtkLabel *filters_warn_label = GTK_LABEL(gtk_builder_get_object(builder, "filters_warn_label"));

	GtkEventBox *crop_corner1 = GTK_EVENT_BOX(gtk_builder_get_object(builder, "crop_corner1"));
	GtkEventBox *crop_corner2 = GTK_EVENT_BOX(gtk_builder_get_object(builder, "crop_corner2"));
	GtkEventBox *crop_corner3 = GTK_EVENT_BOX(gtk_builder_get_object(builder, "crop_corner3"));
	GtkEventBox *crop_corner4 = GTK_EVENT_BOX(gtk_builder_get_object(builder, "crop_corner4"));
	GtkEventBox *crop_corners[5] = {NULL, crop_corner1, crop_corner2, crop_corner3, crop_corner4};

	//---------MENU STRUCT INITIALIZATION---------//
	char orP[100];
	Menu menu =
    {
		.builder = builder, .window = window, .fixed1 = fixed1, .box = box,
        .back_to_menu = back_to_menu, .file_select_grid = file_select_grid,
        .sudoku_image = NULL, .imageOrigin = newPoint(0, 0),
		.grayscale_button = grayscale_button, .gaussian_button = gaussian_button, .sobel_button = sobel_button,
		.autoDetect_button = autoDetect_button, .manuDetect_button = manuDetect_button, .save_button = save_button,
		.resetFilters_button = resetFilters_button, .solve_button = solve_button,
		.rotate_left_button = rotate_left_button, .rotate_right_button = rotate_right_button,
		.crop_corners = crop_corners,
		.filters_grid = filters_grid,
		.originPath = orP, .originImage = NULL,
		.upload_warn_label = upload_warn_label, .filters_warn_label = filters_warn_label
	};

	//-----------------SIGNALS CONNECTION-----------------//

	//Corners
	g_signal_connect(GTK_WIDGET(crop_corner1), "motion-notify-event", G_CALLBACK(on_crop_corners_move), &menu);
	g_signal_connect(GTK_WIDGET(crop_corner2), "motion-notify-event", G_CALLBACK(on_crop_corners_move), &menu);
	g_signal_connect(GTK_WIDGET(crop_corner3), "motion-notify-event", G_CALLBACK(on_crop_corners_move), &menu);
	g_signal_connect(GTK_WIDGET(crop_corner4), "motion-notify-event", G_CALLBACK(on_crop_corners_move), &menu);
	//-------
	g_signal_connect(upload_button, "clicked", G_CALLBACK(on_upload_button_clicked), &menu);
	g_signal_connect(upload_entry, "activate", G_CALLBACK(on_upload_entry_activate), &menu);

	g_signal_connect(back_to_menu, "clicked", G_CALLBACK(on_back_to_menu_button_clicked), &menu);
	g_signal_connect(resetFilters_button, "clicked", G_CALLBACK(on_resetFilters_clicked), &menu);
	g_signal_connect(grayscale_button, "toggled", G_CALLBACK(on_grayscale_toggled), &menu);
	g_signal_connect(gaussian_button, "toggled", G_CALLBACK(refreshImage), &menu);
	g_signal_connect(sobel_button, "toggled", G_CALLBACK(refreshImage), &menu);

	g_signal_connect(rotate_left_button, "clicked", G_CALLBACK(on_rotate_left_clicked), &menu);
	g_signal_connect(rotate_right_button, "clicked", G_CALLBACK(on_rotate_right_clicked), &menu);

	g_signal_connect(autoDetect_button, "clicked", G_CALLBACK(on_autoDetect_clicked), &menu);
	g_signal_connect(manuDetect_button, "clicked", G_CALLBACK(on_manuDetect_clicked), &menu);

	g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_clicked), &menu);
	g_signal_connect(solve_button, "clicked", G_CALLBACK(on_solve_clicked), &menu);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit),NULL);

	//---------WINDOW  INITIALIZATION---------//
	gtk_widget_show(GTK_WIDGET (window));
	gtk_window_set_title(window, "OCR Project");
	gtk_window_set_position(window, GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	gtk_window_set_resizable(window, FALSE);

	//-----------------MAIN LOOP-----------------//
	gtk_main();
	
	//------------------ENDING-------------------//
	freeImage(menu.originImage);
	free(menu.imageOrigin);
	rmDir("tmp/");
	
	return;
}

int rmDir(const char *dir)
{
    int ret = 0;
    FTS *ftsp = NULL;
    FTSENT *curr;
    char *files[] = { (char *) dir, NULL };
    ftsp = fts_open(files, FTS_NOCHDIR | FTS_PHYSICAL | FTS_XDEV, NULL);
    if (!ftsp) {
        ret = -1;
        goto finish;
    }
    while ((curr = fts_read(ftsp))) {
        switch (curr->fts_info) {
        case FTS_NS:
        case FTS_DNR:
        case FTS_ERR:
			ret = -1;
            break;
        case FTS_DC:
        case FTS_DOT:
        case FTS_NSOK:
            break;

        case FTS_D:
            break;

        case FTS_DP:
        case FTS_F:
        case FTS_SL:
        case FTS_SLNONE:
        case FTS_DEFAULT:
            if (remove(curr->fts_accpath) < 0) {
                fprintf(stderr, "%s: Failed to remove: %s\n",
                        curr->fts_path, strerror(curr->fts_errno));
                ret = -1;
            }
            break;
        }
    }

finish:
    if (ftsp) { fts_close(ftsp); }
    return ret;
}