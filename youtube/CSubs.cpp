//
// Created by rhys on 31/07/2021.
//

#include "CSubs.h"

#include "Subscriptions.h"

#include <string>
#include <iostream>

const char* str2chars(std::string str){
    char * writable = new char[str.size() + 1];
    std::copy(str.begin(), str.end(), writable);
    writable[str.size()] = '\0';

    return writable;
}

extern "C" void* channel_new_from_name(const char* name){
    auto* out = Channel::new_from_name(name);

    return (void*) out;
}

extern "C" const char* channel_get_name(void* channel){
    return str2chars( ((Channel*)channel)->get_name());
}

extern "C" const char* channel_get_id(void* channel){
    return str2chars( ((Channel*)channel)->id);
}

extern "C" CVideos channel_get_vids(void* channel){
    std::vector<Video*>* vids = ((Channel*)channel)->get_vids();

    CVideos out;
    out.length = vids->size();
    out.data = new CVideo[out.length];

    for(int i=0 ; i<out.length ; i++){
        auto vid = *vids->at(i);
        out.data[i] = {
                str2chars(vid.link),
                str2chars(vid.title),
                str2chars(vid.channel_name),
                str2chars(vid.channel_id),
                str2chars(vid.description),
                str2chars(vid.publish_date),
                str2chars(vid.thumbnail),
        };
    }

    return out;
}

extern "C" CSubs* subs_new(){
    auto* out = new Subscriptions();

    return (CSubs*) out;
}

extern "C" CVideos subs_get_videos(CSubs *subs) {
    std::vector<Video*>* vids = ((Subscriptions*) subs)->get_vids();

    CVideos out;
    out.length = vids->size();
    out.data = new CVideo[out.length];

    for(int i=0 ; i<out.length ; i++){
        auto vid = *vids->at(i);
        out.data[i] = {
                str2chars(vid.link),
                str2chars(vid.title),
                str2chars(vid.channel_name),
                str2chars(vid.channel_id),
                str2chars(vid.description),
                str2chars(vid.publish_date),
                str2chars(vid.thumbnail),
        };
    }

    return out;
}

extern "C" CFormats cvideo_get_formats(CVideo vid){
    std::vector<Format>* fmts = Video::get_formats(vid.link);

    CFormats out;
    out.length = fmts->size();
    out.data = new CFormat[out.length];

    for(int i=0 ; i<out.length ; i++){
        auto fmt = fmts->at(i);

        out.data[i] = {
                str2chars(fmt.url),
                str2chars(fmt.format)
        };
    }

    return out;
}

extern "C" CChannels subs_get_channels(CSubs* subs){
    std::vector<Channel*>* channels = ((Subscriptions*) subs)->subs;

    CChannels out;
    out.length = channels->size();

    Channel** chans = channels->data();

    out.data = (CChannel**) chans;

    return out;
}

extern "C" void subs_save(CSubs* subs){
    ((Subscriptions* ) subs)->save();
}

extern "C" void subs_remove(CSubs* subs, const char* id){
    ((Subscriptions* ) subs)->remove(std::string(id));
}


extern "C" void cvideos_free(CVideos vids) {
    for(int i=0 ; i<vids.length ; i++){
        CVideo vid = vids.data[i];
        free((void*) vid.link);
        free((void*) vid.title);
        free((void*) vid.channel_name);
        free((void*) vid.channel_id);
        free((void*) vid.description);
        free((void*) vid.publish_date);
        free((void*) vid.thumbnail);
    }
}

extern "C" void cformats_free(CFormats fmts){
    for(int i=0 ; i<fmts.length ; i++){
        CFormat fmt = fmts.data[i];
        free((void*) fmt.format);
        free((void*) fmt.url);
    }
}
