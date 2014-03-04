#include <ux/download.h>
#include <ux/data.h>

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <sstream>

#include <sys/stat.h>


#include "json.h"

struct HttpFile {
    std::string filename;
    FILE *stream;
};

pthread_t download_thread;
pthread_mutex_t downloadMutex; 

int fexists(std::string filename) {
    std::ifstream infile(filename.data());
    return infile.good();
}

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct HttpFile *out=(struct HttpFile *)stream;
    if(out && !out->stream) {
        /* open file for writing */ 
        out->stream=fopen(out->filename.data(), "wb");
        if(!out->stream)
            return -1; /* failure, can't open file to write */ 
    }        
    return fwrite(buffer, size, nmemb, out->stream);
}

void download_file(std::string url, std::string filename) {
    CURL *curl;

    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    std::stringstream ss;


    std::cout << "Descargando " << url << " " << filename << std::endl;

    ss << getenv("HOME") << "/.uxdemo/" << filename;

    struct HttpFile httpFile={
        ss.str(), /* name to store the file as if succesful */ 
        NULL
    };

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &httpFile);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if(CURLE_OK != res) {
            std::cout << "curl told us " << res << std::endl;
        }

        if(httpFile.stream) {
            fclose(httpFile.stream); /* close the local file */ 
        }

    }
    curl_global_cleanup();
}

json_value * read_json(std::string filename) {
    std::stringstream ss;

    ss << getenv("HOME") << "/.uxdemo/" << filename;
    
    std::ifstream in(ss.str().data());
    std::string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    char *errorPos = 0;
    char *errorDesc = 0;
    int errorLine = 0;
    block_allocator allocator(1 << 20); // 1 KB per block

    char * data = (char *)s.data();

    return json_parse(data, &errorPos, (const char **)&errorDesc, &errorLine, &allocator);


}

void download_process() {
    std::stringstream home;
    home << getenv("HOME");
    home << "/.uxdemo";

    mkdir(home.str().data(), 0777);

    home.str("");
    home << getenv("HOME");
    home << "/.uxdemo/twitter";
    mkdir(home.str().data(), 0777);

    home.str("");
    home << getenv("HOME");
    home << "/.uxdemo/instagram";
    mkdir(home.str().data(), 0777);

//     json_value *invalid_tweets = \read_json("list-not-app\roved-tweets.json");
//
//     json_value *invalid_instag\rams = \read_json("list-not-app\roved-instag\rams.json");

    /*
    download_file("http://uxtweet.herokuapp.com/general/background", "background");
    download_file("http://uxtweet.herokuapp.com/api/v1/twitter/list-approved-tweets", "list-approved-tweets.json");
    download_file("http://uxtweet.herokuapp.com/api/v1/twitter/list-not-approved-ids", "list-not-approved-tweets.json");
    download_file("http://uxtweet.herokuapp.com/api/v1/instagram/list-approved-instagrams", "list-approved-instagrams.json");
    download_file("http://uxtweet.herokuapp.com/api/v1/instagram/list-not-approved-links", "list-not-approved-instagrams.json");
    */

    json_value *json_tweets = read_json("list-approved-tweets.json");
    tweets.clear();

    /*
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
            tweets.push_back(twitt);


            home.str("");
            home << getenv("HOME") << "/.uxdemo/twitter/" << twitter_id;
            mkdir(home.str().data(), 0777);

            if(slug.size() > 0) {
                stringstream picture;
                picture << "http://uxtweet.herokuapp.com/api/v1/twitter/picture/";
                picture << slug;
                home.str("");
                home << "twitter/" << twitter_id << "/slug";
                if(!fexists(picture.str().data())) {
                    download_file(picture.str().data(), home.str().data());
                }
            }

            if(picture_url.size() > 0) {
                home.str("");
                home << "twitter/" << twitter_id << "/picture_url";
                if(!fexists(picture_url)) {
                    download_file(picture_url, home.str().data());
                }
            }

        }
    }
    */

    json_value *json_instagrams = read_json("list-approved-instagrams.json");
    instagrams.clear();


    for (json_value *it = json_instagrams->first_child; it; it = it->next_sibling) {
        if(it->type == JSON_OBJECT) {
            int video;
            std::string link;
            std::string name;
            std::string profile_url;
            std::string standard_resolution;
            std::string tags = "";

            for (json_value *it1 = it->first_child; it1; it1 = it1->next_sibling) {
                std::cout << it1->name << std::endl;
                if(it1->type == JSON_NULL) {
                    continue;
                }
                if(strcmp(it1->name, "link") == 0) {
                    char *s = (char *)malloc(strlen(it1->string_value) + 1);
                    strcpy(s, it1->name);
                    link = s;
                }
                if(strcmp(it1->name, "name") == 0) {
                    char *s = (char *)malloc(strlen(it1->string_value) + 1);
                    strcpy(s, it1->name);
                    name = s;
                }
                if(strcmp(it1->name, "profile_url") == 0) {
                    char *s = (char *)malloc(strlen(it1->string_value) + 1);
                    strcpy(s, it1->name);
                    profile_url = s;
                }
                if(strcmp(it1->name, "standard_resolution") == 0) {
                    char *s = (char *)malloc(strlen(it1->string_value) + 1);
                    strcpy(s, it1->name);
                    standard_resolution = s;
                }
                if(strcmp(it1->name, "video") == 0) {
                    video = it1->int_value;
                }
            }


            std::cout << link.size() << std::endl;
            if(link.size() >= 24) {
                Instagram *instagram = new Instagram(video, link, name, profile_url, standard_resolution, tags);
                instagrams.push_back(instagram);

                home.str("");
                home << getenv("HOME") << "/.uxdemo/instagram/" << link.substr(24, 9);
                mkdir(home.str().data(), 0777);

                if(profile_url.size() > 0) {
                    home.str("");
                    home << "instagram/" << link.substr(24, 9) << "/profile_url";
                    if(!fexists(profile_url)) {
                        download_file(profile_url, home.str().data());
                    }
                }

                if(standard_resolution.size() > 0) {
                    home.str("");
                    home << "instagram/" << link.substr(24, 9) << "/standard_resolution";
                    if(!fexists(home.str().data())) {
                        download_file(standard_resolution, home.str().data());
                    }
                }
            }

        }
    }

    
}

void *do_download(void*) {
    while(true) {
        pthread_mutex_lock(&downloadMutex);

        std::cout << "Downloading files" << std::endl;

        download_process();

        std::cout << "Finished downloading" << std::endl;

    }
}

void init_download() {
    pthread_mutex_init(&downloadMutex, NULL);
    pthread_create( &download_thread, NULL, do_download, NULL);
}

void download() {
    pthread_mutex_unlock(&downloadMutex);
}
