#ifndef http_h
#define http_h
#include <curl/curl.h>

CURL* httpget(const char* url, struct curl_slist* headers);

#endif