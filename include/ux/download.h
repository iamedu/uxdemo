#pragma once

#include <curl/curl.h>
#include <pthread.h>
#include <time.h>

void init_download();
void download();
