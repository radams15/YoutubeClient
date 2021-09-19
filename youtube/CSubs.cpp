//
// Created by rhys on 31/07/2021.
//

#include "CSubs.h"

#include "Subscriptions.h"

#include <string>

const char* str2chars(std::string str){
    char * writable = new char[str.size() + 1];
    std::copy(str.begin(), str.end(), writable);
    writable[str.size()] = '\0';

    return writable;
}

extern "C" CSubs* subs_new(const char* file){
    auto* out = new Subscriptions(std::string(file));

    return (CSubs*) out;
}

extern "C" CVideos subs_get_videos(CSubs *subs) {
    std::vector<Video*>* vids = ((Subscriptions*) subs)->get_vids();

    CVideos out;
    out.length = vids->size();
    out.vids = new CVideo[out.length];

    for(int i=0 ; i<out.length ; i++){
        auto vid = *vids->at(i);
        out.vids[i] = {
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
    out.formats = new CFormat[out.length];

    for(int i=0 ; i<out.length ; i++){
        auto fmt = fmts->at(i);
        out.formats[i] = {
                str2chars(fmt.url),
                str2chars(fmt.format)
        };
    }

    return out;
}

extern "C" void cvideos_free(CVideos vids) {
    for(int i=0 ; i<vids.length ; i++){
        CVideo vid = vids.vids[i];
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
        CFormat fmt = fmts.formats[i];
        free((void*) fmt.format);
        free((void*) fmt.url);
    }
}