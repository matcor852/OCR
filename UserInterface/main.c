#include "ui.h"

int main (int argc, char *argv[])
{
	gtk_init(&argc, &argv); //init Gtk
	initUserInterface(); //init UserInterface
	return 0;
}