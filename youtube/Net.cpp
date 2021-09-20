//
// Created by rhys on 20/09/2021.
//

#include <curl/curl.h>

#include <iostream>

#include "Net.h"

size_t resp_callback(void *contents, size_t size, size_t nmemb, Net::Resp* resp) {
    size_t newLength = size*nmemb;

    try {
        resp->text.append((char*)contents, newLength);
    } catch(std::bad_alloc &e) {
        return 0;
    }

    return newLength;
}

Net::Resp Net::get(std::string url, bool debug) {
    CURL *curl;
    CURLcode res;
    Resp out;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //only for https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); //only for https
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, resp_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
        curl_easy_setopt (curl, CURLOPT_VERBOSE, (long) debug); //remove this to disable verbose output

        res = curl_easy_perform(curl);

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(out.status_code));

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    return out;
}
