//
// Created by rhys on 31/07/2021.
//

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char* link;
    const char* title;
    const char* channel_name;
    const char* channel_id;
    const char* description;
    const char* publish_date;
    const char* thumbnail;
} CVideo;

typedef struct {
    CVideo* data;
    int length;
} CVideos;

typedef struct {
    const char* url;
    const char* format;
} CFormat;

typedef struct {
    CFormat* data;
    int length;
} CFormats;

typedef void CSubs;
typedef void CChannel;

typedef struct {
    CChannel** data;
    int length;
} CChannels;

void* channel_new_from_name(const char* name);
CVideos channel_get_vids(void* channel);
const char* channel_get_name(void* channel);
const char* channel_get_id(void* channel);

CSubs* subs_new(const char* file);
CChannels subs_get_channels(CSubs* subs);
CVideos subs_get_videos(CSubs* subs);
CFormats cvideo_get_formats(CVideo vid);
void subs_save(CSubs* subs);
void subs_remove(CSubs* subs, const char* id);

void cvideos_free(CVideos vids);
void cformats_free(CFormats fmts);

#ifdef __cplusplus
}
#endif