#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "http.h"

static struct curl_slist* alpaca_build_auth_headers(void) {
    const char* api_key = getenv("ALPACA_API_KEY");
    const char* api_secret = getenv("ALPACA_API_SECRET_KEY");

    if (!api_key || !api_secret) {
        return NULL;
    }

    char header_key[128];
    char header_secret[128];
    snprintf(header_key, sizeof header_key, "APCA-API-KEY-ID: %s", api_key);
    snprintf(header_secret, sizeof header_secret, "APCA-API-SECRET-KEY: %s", api_secret);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, header_key);
    headers = curl_slist_append(headers, header_secret);
    return headers;
}

void alpaca_get_account(char* response, size_t response_size) {
    struct curl_slist *headers = alpaca_build_auth_headers();
    if (!headers) {
        snprintf(response, response_size, "missing API credentials");
        return;
    }

    httpget("https://paper-api.alpaca.markets/v2/account", headers, response, response_size);
    curl_slist_free_all(headers);
}

void alpaca_get_latest_bars(const char* ticker, char* response, size_t response_size) {
    struct curl_slist *headers = alpaca_build_auth_headers();
    if (!headers) {
        snprintf(response, response_size, "missing API credentials");
        return;
    }

    char url[256];
    snprintf(url, sizeof url,
             "https://data.alpaca.markets/v2/stocks/bars/latest?symbols=%s",
             ticker);
    httpget(url, headers, response, response_size);
    curl_slist_free_all(headers);
}