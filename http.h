#ifndef http_h
#define http_h
#include <curl/curl.h>

void httpget(const char* url, struct curl_slist* headers, char* response, size_t response_size);
void httppost(const char* url, struct curl_slist* headers, const char* post_data, char* response, size_t response_size);

#endif