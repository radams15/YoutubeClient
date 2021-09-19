//
// Created by rhys on 14/09/2021.
//

#ifndef C_MAINAPPLICATION_H
#define C_MAINAPPLICATION_H

#include <gtk/gtk.h>
#include <libadwaita-1/adwaita.h>

static void activate(GtkApplication* app, gpointer user_data);

int app_run(int argc, char **argv);

#endif //C_MAINAPPLICATION_H
