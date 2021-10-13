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
    
    std::cout << file << ", " << subs.length() << "\n";
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


typedef struct {
    Channel* c;
    std::vector<Video*>* out;
} VidData;

static pthread_mutex_t mutex;

void* get_vid(void* ptr){
    auto* v = (VidData*) ptr;

    pthread_mutex_lock(&mutex);

    auto c_vids = v->c->get_vids();

    v->out->insert(v->out->end(), c_vids->begin(), c_vids->end());

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

std::vector<Video*>* Subscriptions::get_vids() {
    auto out = new std::vector<Video*>;

    std::vector<pthread_t> threads;

    for(auto c : *subs){
        threads.push_back(0);

        auto* v = new VidData;
        v->c = c;
        v->out = out;

        pthread_create(&threads[threads.size()-1], NULL, get_vid, v);
    }

    for(auto t : threads){
        void* status;
        pthread_join(t, &status);
    }

    sort(out->begin(), out->end(), [](Video* a, Video* b) -> bool{
        return a->publish_date < b->publish_date;
    });

    return out;
}

void Subscriptions::remove(std::string id) {
    auto it = std::remove_if(subs->begin(), subs->end(), [id](Channel* chan){
        return chan->id == id;
    });
    subs->erase(it, subs->end());
}
