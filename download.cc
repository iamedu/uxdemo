#include <ux/download.h>

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <sstream>

#include <sys/stat.h>

struct HttpFile {
    std::string filename;
    FILE *stream;
};

pthread_t download_thread;
pthread_mutex_t downloadMutex; 

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

    ss << getenv("HOME");
    ss << "/.uxdemo/";
    ss << "hola.png";

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

    }
    curl_global_cleanup();
}

void download_process() {
    std::stringstream home;
    home << getenv("HOME");

    home << "/.uxdemo";

    mkdir(home.str().data(), 0777);
    download_file("http://uxtweet.herokuapp.com/general/background", "background.png");

}

void *do_download(void*) {
    while(true) {
        pthread_mutex_lock(&downloadMutex);

        std::cout << "Downloading files" << std::endl;

        download_process();

    }
}

void init_download() {
    pthread_mutex_init(&downloadMutex, NULL);
    pthread_create( &download_thread, NULL, do_download, NULL);
}

void download() {
    pthread_mutex_unlock(&downloadMutex);
}
