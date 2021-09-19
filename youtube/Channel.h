//
// Created by rhys on 27/07/2021.
//

#ifndef C_CHANNEL_H
#define C_CHANNEL_H

#include <string>
#include <regex>
#include <vector>

#include "Video.h"

#define ID_REGEX R"(\"externalId\":\"([0-9a-zA-Z_\-]*)\")"
#define NAME_REGEX R"(<name>(.*)<\/name>)"

class Channel {
private:

    std::string name;

public:
    std::string id;

    Channel(std::string id);

    static Channel* new_from_name(std::string name);

    std::string get_name();

    std::vector<Video*>* get_vids();
};


#endif //C_CHANNEL_H
