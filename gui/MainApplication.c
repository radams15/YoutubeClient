//
// Created by rhys on 14/09/2021.
//

#include "MainApplication.h"

#include <Config.h>

#include <gio/gio.h>
#include <libadwaita-1/adwaita.h>

#include <stdio.h>


GtkBuilder* builder;
Config_t* conf;

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
    const char* url = video_get_playable(((Video_t*) data), conf);

    if(strlen(url) > 0) {
        play_video(url);
    }else{
        fprintf(stderr, "Cannot get any formats for video %s\n", ((Video_t*) data)->id);
    }
}

void del_sub_button_clicked(GtkButton* button, gpointer data){
    //config_subs_remove(conf, (const char*) data);
}

const char* date_str(long date){
    char* buf = calloc(128, sizeof(char));

    sprintf(buf, "%d", date);

    return buf;
}

void add_feed_item(Video_t* vid, void* ptr){
    GtkBuilder* new_builder = gtk_builder_new_from_file(INSTALLED"share/Youtube/FeedBox.ui");

    GtkWidget* box = GTK_WIDGET(gtk_builder_get_object(new_builder, "main_box"));

    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(new_builder, "author_label")), vid->channel_name);
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(new_builder, "title_label")), vid->title);
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(new_builder, "date_label")), date_str(vid->published));
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(new_builder, "date_label_hidden")), date_str(vid->published));

    GtkButton* playbtn = GTK_BUTTON(gtk_builder_get_object(new_builder, "play_btn"));

    g_signal_connect(playbtn, "clicked", G_CALLBACK (play_video_button_clicked), (gpointer) vid);

    gtk_list_box_prepend(GTK_LIST_BOX(gtk_builder_get_object(builder, "feed_box")), box);
}

void add_sub_item(Channel_t* chan){
    GtkBuilder* new_builder = gtk_builder_new_from_file(INSTALLED"share/Youtube/SubBox.ui");

    GtkWidget* box = GTK_WIDGET(gtk_builder_get_object(new_builder, "main_box"));

    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(new_builder, "channel_name")), chan->name);

    GtkButton* delbtn = GTK_BUTTON(gtk_builder_get_object(new_builder, "delete_btn"));

    g_signal_connect(delbtn, "clicked", G_CALLBACK (del_sub_button_clicked), (gpointer) chan->id);

    gtk_list_box_append(GTK_LIST_BOX(gtk_builder_get_object(builder, "sub_box")), box);
}

void load_feed(){
    config_get_vids(conf, add_feed_item, NULL);

    /*
   Videos_t* vids = config_get_vid_list(conf);
    printf("Vids: %d\n", vids->length);

    for(int i=0 ; i<vids->length ; i++){
        add_feed_item(vids->arry[i]);
    }*/
}

void load_subs(){
    for(int i=0 ; i<conf->subs->length ; i++){
        Channel_t* channel = conf->subs->arry[i];
        add_sub_item(channel);
    }
}

int get_row_date(GtkListBoxRow* row){
    GtkWidget* box = gtk_list_box_row_get_child(row);

    GtkLabel* date_label = gtk_widget_get_first_child(box);

    const char* date = gtk_label_get_text(date_label);

    return atol(date);
}

gint box_sorter(GtkListBoxRow* a, GtkListBoxRow* b, gpointer ptr){
    int d1 =  get_row_date(a);

    int d2 = get_row_date(b);

    return d1 < d2;
}

void activate (GtkApplication* app, gpointer user_data){
    builder = gtk_builder_new_from_file(INSTALLED"share/Youtube/MainWindow.ui");

    GtkWindow* win = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    AdwSqueezer* squeezer = ADW_SQUEEZER(gtk_builder_get_object(builder, "squeezer"));
    g_signal_connect(squeezer, "notify::visible-child", G_CALLBACK (on_headerbar_squeezer_notify), NULL);

    gtk_window_set_application(win, app);
    gtk_window_present(win);

    const char* conf_file = "/home/rhys/" CONF_DIR;

    conf = config_new(conf_file, FALSE);

    load_feed();
    load_subs();

    gtk_list_box_set_sort_func(GTK_LIST_BOX(gtk_builder_get_object(builder, "feed_box")), box_sorter, NULL, NULL);

    //gtk_list_box_invalidate_sort(GTK_LIST_BOX(gtk_builder_get_object(builder, "feed_box")));
}

int app_run (int argc, char **argv){
    GtkApplication *app;
    int status;

    adw_init();

    app = gtk_application_new ("org.chev.Youtube", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    config_save(conf);

    return status;
}
