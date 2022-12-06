#include "ui.h"

int main (int argc, char *argv[])
{
	gtk_init(&argc, &argv); //init Gtk
	initTrig(); //init trigonometric tables
	initUserInterface(); //init UserInterface
	return 0;
}