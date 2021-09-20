//
// Created by rhys on 14/09/2021.
//

#include "MainApplication.h"
#include "../youtube/CSubs.h"

#include <gio/gio.h>

#include <stdio.h>
#include <stdarg.h>

GtkBuilder* builder;
CSubs* subs;

void on_headerbar_squeezer_notify(AdwSqueezer* squeezer, GdkEvent* event){
    GtkWidget* child = adw_squeezer_get_visible_child(ADW_SQUEEZER(gtk_builder_get_object(builder, "squeezer")));

    gboolean reveal = FALSE;

    if(child != (GtkWidget*) gtk_builder_get_object(builder, "headerbar_switcher")) {
        reveal = TRUE;
    }

    GtkWidget* bottom_switcher = (GtkWidget *) gtk_builder_get_object(builder, "bottom_switcher");

    if(ADW_IS_VIEW_SWITCHER_BAR (bottom_switcher)){
        adw_view_switcher_bar_set_reveal(ADW_VIEW_SWITCHER_BAR(bottom_switcher), reveal);
    }
}

void play_video(const char* url){
    GFile* file = g_file_new_for_uri(url);
    GtkMediaStream* stream = gtk_media_file_new_for_file(file);

    adw_view_stack_set_visible_child_name(ADW_VIEW_STACK(gtk_builder_get_object(builder, "main_stack")),"player_page");

    gtk_video_set_media_stream(GTK_VIDEO(gtk_builder_get_object(builder, "player")), stream);
    gtk_video_set_autoplay(GTK_VIDEO(gtk_builder_get_object(builder, "player")), true);
}

void play_video_button_clicked(GtkButton* button, gpointer data){
    CFormats fmts = cvideo_get_formats(*((CVideo*) data));

    play_video(fmts.formats[0].url);
}

void add_feed_item(CVideo* vid){
    GtkBuilder* new_builder = gtk_builder_new_from_file("../ui/FeedBox.ui");

    GtkWidget* box = GTK_WIDGET(gtk_builder_get_object(new_builder, "main_box"));

    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(new_builder, "author_label")), vid->channel_name);
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(new_builder, "title_label")), vid->title);
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(new_builder, "date_label")), vid->publish_date);

    GtkButton* playbtn = GTK_BUTTON(gtk_builder_get_object(new_builder, "play_btn"));

    g_signal_connect(playbtn, "clicked", G_CALLBACK (play_video_button_clicked), (gpointer) vid);

    gtk_list_box_prepend(GTK_LIST_BOX(gtk_builder_get_object(builder, "feed_box")), box);
}

void load_feed(){
    CVideos vids = subs_get_videos(subs);

    for(int i=0 ; i<vids.length ; i++){
        add_feed_item(&(vids.vids[i]));
    }
}

void activate (GtkApplication* app, gpointer user_data){
    builder = gtk_builder_new_from_file("../ui/MainWindow.ui");

    GtkWindow* win = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    AdwSqueezer* squeezer = ADW_SQUEEZER(gtk_builder_get_object(builder, "squeezer"));
    g_signal_connect(squeezer, "notify::visible-child", G_CALLBACK (on_headerbar_squeezer_notify), NULL);

    gtk_window_set_application(win, app);
    gtk_window_present(win);

    load_feed();
}

int app_run (int argc, char **argv){
    GtkApplication *app;
    int status;

    subs = subs_new("../subs.json");

    adw_init();

    app = gtk_application_new ("org.chev.Youtube", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
