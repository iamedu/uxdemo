#pragma once

#include <vector>
#include <string>

using namespace std;


class Twitt {
    public:
        string status;
        string slug;
        string picture_url;
        string name;
        string twitter_id;
        Twitt(string status, string slug, string picture_url, string name, string twitter_id) :
            status(status), slug(slug), picture_url(picture_url), name(name), twitter_id(twitter_id) {
        }
};

class Instagram {
    public:
        int video;
        string link;
        string name;
        string profile_url;
        string standard_resolution;
        string tags;
        
        Instagram(int video, string link, string name, string profile_url, string standard_resolution, string tags) :
            video(video), name(name), profile_url(profile_url), standard_resolution(standard_resolution), tags(tags) {
        }
};

void load_data(vector<Twitt *> *tweets, vector<Instagram *> *instagrams);
