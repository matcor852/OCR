#include "menu.h"

int main (int argc, char *argv[])
{
	gtk_init(&argc, &argv); //init Gtk
	startMenu();
	return EXIT_SUCCESS;
}