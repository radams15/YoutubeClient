//
// Created by rhys on 27/07/2021.
//

#include "Channel.h"

#include <iostream>

#include <libxml++/libxml++.h>
#include <libxml/parser.h>

#include "Net.h"

Channel* Channel::new_from_name(std::string name) {
    Net::Resp res = Net::get("https://www.youtube.com/c/"+name+"/featured");

    if(res.status_code != 200){
        std::cerr << "FAIL to get channel id URL with error code: " << res.status_code << std::endl;
        return nullptr;
    }


    std::regex id_regex(ID_REGEX);

    std::smatch id_match;

    if(std::regex_search(res.text, id_match, id_regex)){
        return new Channel(id_match[1]);
    }

    std::cerr << "FAIL to get channel id regex match" << std::endl;

    return nullptr;
}

Channel::Channel(std::string id) {
    this->id = id;
    this->name = "";
}

std::string Channel::get_name() {
    if(name.empty()) {
        Net::Resp res = Net::get("https://www.youtube.com/feeds/videos.xml?channel_id=" + id);


        if(res.status_code != 200) {
            std::cerr << "FAIL to get channel name URL" << std::endl;
            return "";
        }

        std::regex name_regex(NAME_REGEX);

        std::smatch name_match;

        if(std::regex_search(res.text, name_match, name_regex)) {
            return name_match[1];
        }

        return "";
    }

    return name;
}

void make_video(const xmlpp::Node* node, Video* vid, int level){
    for(auto i : node->get_children()){
        auto key = i->get_name();

        //const auto nodeText = dynamic_cast<const xmlpp::TextNode*>(i);
        //const auto nodeComment = dynamic_cast<const xmlpp::CommentNode*>(i);
        //const auto nodeContent = dynamic_cast<const xmlpp::ContentNode*>(i);
        //const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(i)

        if(key == "group"){
            make_video(i, vid, level+1);
        } else if(key == "link"){
            auto* nodeElement = dynamic_cast<const xmlpp::Element*>(i);
            vid->link = nodeElement->get_attribute("href")->get_value();
        } else if(key == "published"){
            vid->publish_date = std::string((const char*) xmlNodeGetContent(i->cobj()));
        } else if(key == "description"){
            vid->description = std::string((const char*) xmlNodeGetContent(i->cobj()));
        } else if(key == "thumbnail"){
            auto* nodeElement = dynamic_cast<const xmlpp::Element*>(i);
            vid->thumbnail = nodeElement->get_attribute("url")->get_value();
        } else if(key == "title"){
            vid->title = std::string((const char*) xmlNodeGetContent(i->cobj()));
        }
    }
}

std::vector<Video*>* Channel::get_vids() {
    auto* out = new std::vector<Video*>;

    Net::Resp res = Net::get("https://www.youtube.com/feeds/videos.xml?channel_id=" + id);


    if(res.status_code != 200){
        std::cerr << "FAIL to get vids URL: RETURN " << res.status_code << std::endl;
        return nullptr;
    }

    xmlpp::DomParser p;
    p.parse_memory(res.text);

    auto root = p.get_document()->get_root_node();

    for(auto i : root->get_children()){
        if(i->get_name() == "entry"){
            auto* vid = new Video;

            make_video(i, vid, 0);

            out->push_back(vid);
        }
    }

    return out;
}
