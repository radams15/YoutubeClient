//
// Created by rhys on 14/09/2021.
//

#include "MainApplication.h"
#include "../youtube/CSubs.h"

#include <gio/gio.h>

#include <stdio.h>

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

    gtk_video_set_media_stream(GTK_VIDEO(gtk_builder_get_object(builder, "player")), stream);
    gtk_video_set_autoplay(GTK_VIDEO(gtk_builder_get_object(builder, "player")), true);

    adw_view_stack_set_visible_child_name(ADW_VIEW_STACK(gtk_builder_get_object(builder, "main_stack")),"player_page");
}

void play_video_button_clicked(GtkButton* button, gpointer data){
    CFormats fmts = cvideo_get_formats(*((CVideo*) data));

    if(fmts.length > 0) {
        play_video(fmts.data[0].url);
    }else{
        fprintf(stderr, "Cannot get any formats for video %s\n", ((CVideo*) data)->link);
    }
}

void del_sub_button_clicked(GtkButton* button, gpointer data){
    subs_remove(subs, (const char*) data);
}

void add_feed_item(CVideo* vid){
    GtkBuilder* new_builder = gtk_builder_new_from_file("/usr/local/share/Youtube/FeedBox.ui");

    GtkWidget* box = GTK_WIDGET(gtk_builder_get_object(new_builder, "main_box"));

    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(new_builder, "author_label")), vid->channel_name);
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(new_builder, "title_label")), vid->title);
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(new_builder, "date_label")), vid->publish_date);

    GtkButton* playbtn = GTK_BUTTON(gtk_builder_get_object(new_builder, "play_btn"));

    g_signal_connect(playbtn, "clicked", G_CALLBACK (play_video_button_clicked), (gpointer) vid);

    gtk_list_box_prepend(GTK_LIST_BOX(gtk_builder_get_object(builder, "feed_box")), box);
}

void add_sub_item(CChannel* chan){
    GtkBuilder* new_builder = gtk_builder_new_from_file("/usr/local/share/Youtube/SubBox.ui");

    GtkWidget* box = GTK_WIDGET(gtk_builder_get_object(new_builder, "main_box"));

    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(new_builder, "channel_name")), channel_get_name(chan));

    GtkButton* delbtn = GTK_BUTTON(gtk_builder_get_object(new_builder, "delete_btn"));

    g_signal_connect(delbtn, "clicked", G_CALLBACK (del_sub_button_clicked), (gpointer) channel_get_id(chan));

    gtk_list_box_append(GTK_LIST_BOX(gtk_builder_get_object(builder, "sub_box")), box);
}

void load_feed(){
    CVideos vids = subs_get_videos(subs);

    for(int i=0 ; i<vids.length ; i++){
        add_feed_item(&(vids.data[i]));
    }
}

void load_subs(){
    CChannels channels = subs_get_channels(subs);

    for(int i=0 ; i<channels.length ; i++){
        CChannel* channel = channels.data[i];
        add_sub_item(channel);
    }
}

void activate (GtkApplication* app, gpointer user_data){
    builder = gtk_builder_new_from_file("/usr/local/share/Youtube/MainWindow.ui");

    GtkWindow* win = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    AdwSqueezer* squeezer = ADW_SQUEEZER(gtk_builder_get_object(builder, "squeezer"));
    g_signal_connect(squeezer, "notify::visible-child", G_CALLBACK (on_headerbar_squeezer_notify), NULL);

    gtk_window_set_application(win, app);
    gtk_window_present(win);

    /*load_feed();
    load_subs();*/
}

int app_run (int argc, char **argv){
    GtkApplication *app;
    int status;

    //subs = subs_new("~/.var/app/com.chev.Youtube/data/subs.json");

    adw_init();

    app = gtk_application_new ("org.chev.Youtube", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    subs_save(subs);

    return status;
}
