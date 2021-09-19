//
// Created by rhys on 27/07/2021.
//

#include "Subscriptions.h"

#include <filesystem>
#include <fstream>
#include <iostream>


namespace fs = std::filesystem;

Subscriptions::Subscriptions(std::string file) {
    this->file = file;
    this->subs = new std::vector<Channel*>;

    if(fs::exists(file)){
        load();
    }
}

void Subscriptions::load() {
    std::ifstream in(file);

    json j;

    in >> j;

    for(auto id : j){
        subs->push_back(new Channel(id));
    }
}

void Subscriptions::save() {
    std::ofstream out(file);

    json j = json::array();

    for(auto c : *subs){
        j.push_back(c->id);
    }

    out << std::setw(4) << j << std::endl;
}

std::vector<Video*>* Subscriptions::get_vids() {
    auto out = new std::vector<Video*>;

    for(auto c : *subs){
        auto c_vids = c->get_vids();

        out->insert(out->end(), c_vids->begin(), c_vids->end());
    }

    sort(out->begin(), out->end(), [](Video* a, Video* b) -> bool{
        return a->publish_date < b->publish_date;
    });

    return out;
}
