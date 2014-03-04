#include <ux/data.h>
#include <ux/download.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>

#include "json.h"

json_value * read_json(std::string filename);
int file_exists(std::string filename);

void load_data(vector<Twitt* > *tweets, vector<Instagram *> *instagrams) {
    std::stringstream home;

    if(!file_exists("list-approved-tweets.json")) {
        return;
    }

    json_value *json_tweets = read_json("list-approved-tweets.json");
    if(!json_tweets) {
        return;
    }
    tweets->clear();

    for (json_value *it = json_tweets->first_child; it; it = it->next_sibling) {
        if(it->type == JSON_OBJECT) {
            std::string status;
            std::string slug;
            std::string picture_url;
            std::string name;
            std::string twitter_id;

            for (json_value *it1 = it->first_child; it1; it1 = it1->next_sibling) {
                if(it1->type == JSON_NULL) {
                    continue;
                }
                if(strcmp(it1->name, "status") == 0) {
                    status = it1->string_value;
                }
                if(strcmp(it1->name, "slug") == 0) {
                    slug = it1->string_value;
                }
                if(strcmp(it1->name, "picture_url") == 0) {
                    picture_url = it1->string_value;
                }
                if(strcmp(it1->name, "name") == 0) {
                    name = it1->string_value;
                }
                if(strcmp(it1->name, "twitter_id") == 0) {
                    twitter_id = it1->string_value;
                }
            }


            Twitt *twitt = new Twitt(status, slug, picture_url,name, twitter_id);
            tweets->push_back(twitt);

        }
    }

    if(!file_exists("list-approved-instagrams.json")) {
        return;
    }

    json_value *json_instagrams = read_json("list-approved-instagrams.json");
    if(!json_instagrams) {
        return;
    }
    instagrams->clear();


    for (json_value *it = json_instagrams->first_child; it; it = it->next_sibling) {
        if(it->type == JSON_OBJECT) {
            int video;
            std::string link;
            std::string name;
            std::string profile_url;
            std::string standard_resolution;
            std::string tags = "";

            for (json_value *it1 = it->first_child; it1; it1 = it1->next_sibling) {
                if(it1->type == JSON_NULL) {
                    continue;
                }
                if(strcmp(it1->name, "link") == 0) {
                    char *s = (char *)malloc(strlen(it1->string_value) + 1);
                    strcpy(s, it1->string_value);
                    link = s;
                }
                if(strcmp(it1->name, "name") == 0) {
                    char *s = (char *)malloc(strlen(it1->string_value) + 1);
                    strcpy(s, it1->string_value);
                    name = s;
                }
                if(strcmp(it1->name, "profile_url") == 0) {
                    char *s = (char *)malloc(strlen(it1->string_value) + 1);
                    strcpy(s, it1->string_value);
                    profile_url = s;
                }
                if(strcmp(it1->name, "standard_resolution") == 0) {
                    char *s = (char *)malloc(strlen(it1->string_value) + 1);
                    strcpy(s, it1->string_value);
                    standard_resolution = s;
                }
                if(strcmp(it1->name, "video") == 0) {
                    video = it1->int_value;
                }
            }

            Instagram *instagram = new Instagram(video, link, name, profile_url, standard_resolution, tags);
            instagrams->push_back(instagram);
        }
    }
}
