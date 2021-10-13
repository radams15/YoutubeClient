//
// Created by rhys on 27/07/2021.
//

#ifndef C_SUBSCRIPTIONS_H
#define C_SUBSCRIPTIONS_H

#include "json.hpp"
#include "Channel.h"

#include <string>
#include <vector>

using json=nlohmann::json;

class Subscriptions {
private:
    std::string file;

    void load();

public:
    void save();

    std::vector<Channel*>* subs;

    Subscriptions(std::string file);

    std::vector<Video*>* get_vids();

    void remove(std::string id);
};


#endif //C_SUBSCRIPTIONS_H
