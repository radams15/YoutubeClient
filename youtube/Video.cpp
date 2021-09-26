//
// Created by rhys on 27/07/2021.
//

#include "Video.h"

#include <cstdio>
#include <iostream>
#include <memory>

#include "json.hpp"

using json=nlohmann::json;

std::string exec(std::string cmd) {
    std::array<char, COMMAND_BUF> buffer;
    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}


std::vector<Format>* Video::get_formats() {
    return get_formats(link);
}

std::vector<Format> *Video::get_formats(std::string link) {
    auto* out = new std::vector<Format>;

    std::string output = exec("yt-dlp -j " + link);

    json info = json::parse(output);

    for(auto form : info["formats"]){
        std::string acodec = form["acodec"];
        std::string vcodec = form["vcodec"];

        if(acodec == "none" or vcodec == "none" or vcodec == "mp4v.20.3"){
            continue;
        }

        out->push_back(Format{form["url"], form["format_note"]});
    }

    return out;
}
