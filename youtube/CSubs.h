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
    CVideo* vids;
    int length;
} CVideos;

typedef struct {
    const char* url;
    const char* format;
} CFormat;

typedef struct {
    CFormat* formats;
    int length;
} CFormats;

typedef void CSubs;
typedef void CChannel;

CChannel* channel_new_from_name(const char* name);
CVideos channel_get_vids(CChannel* channel);

CSubs* subs_new(const char* file);

CVideos subs_get_videos(CSubs* subs);

CFormats cvideo_get_formats(CVideo vid);

void cvideos_free(CVideos vids);
void cformats_free(CFormats fmts);

#ifdef __cplusplus
}
#endif