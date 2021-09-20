//
// Created by rhys on 20/09/2021.
//

#ifndef YOUTUBE_NET_H
#define YOUTUBE_NET_H

#include <string>

namespace Net {
    struct Resp {
        long status_code;
        std::string text;
    };

    Resp get(std::string url, bool debug=false);
}


#endif //YOUTUBE_NET_H
