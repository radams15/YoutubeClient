//
// Created by rhys on 27/07/2021.
//

#ifndef C_VIDEO_H
#define C_VIDEO_H

#include <string>
#include <vector>

#define COMMAND_BUF 128

struct Format{
    std::string url;
    std::string format;
};


struct Video {
    std::string link;
    std::string title;
    std::string channel_name;
    std::string channel_id;
    std::string description;
    std::string publish_date;
    std::string thumbnail;

    std::vector<Format>* get_formats();
    static std::vector<Format>* get_formats(std::string link);
};


#endif //C_VIDEO_H
