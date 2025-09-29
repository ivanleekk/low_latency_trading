#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"
#include "http.h"
#include "alpaca.h"


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

void alpaca_post_order(const char* order_json, char* response, size_t response_size) {
    struct curl_slist *headers = alpaca_build_auth_headers();
    if (!headers) {
        snprintf(response, response_size, "missing API credentials");
        return;
    }

    headers = curl_slist_append(headers, "Content-Type: application/json");
    httppost("https://paper-api.alpaca.markets/v2/orders", headers, order_json, response, response_size);
    curl_slist_free_all(headers);
}

void alpaca_create_order_json(const char* symbol, int qty, const char* side, const char* type, const char* time_in_force, char* order_json, size_t order_json_size) {
    snprintf(order_json, order_json_size,
             "{\"symbol\":\"%s\",\"qty\":%d,\"side\":\"%s\",\"type\":\"%s\",\"time_in_force\":\"%s\"}",
             symbol, qty, side, type, time_in_force);
}

Bars alpaca_decode_bars_json(char* json) {
    Bars bars = {0}; // Initialize to zero

    cJSON *root = cJSON_Parse(json);
    if (!root) {
        printf("Error parsing JSON\n");
        return bars;
    }
    
    cJSON *bars_obj = cJSON_GetObjectItem(root, "bars");
    if (!bars_obj) {
        printf("No 'bars' object found\n");
        cJSON_Delete(root);
        return bars;
    }
    
    cJSON *symbol_obj = bars_obj->child;
    if (!symbol_obj) {
        printf("No symbol data found\n");
        cJSON_Delete(root);
        return bars;
    }
    
    strncpy(bars.symbol, symbol_obj->string, sizeof(bars.symbol) - 1);
    bars.symbol[sizeof(bars.symbol) - 1] = '\0';
    
    cJSON *close = cJSON_GetObjectItem(symbol_obj, "c");
    cJSON *high = cJSON_GetObjectItem(symbol_obj, "h");
    cJSON *low = cJSON_GetObjectItem(symbol_obj, "l");
    cJSON *trade_count = cJSON_GetObjectItem(symbol_obj, "n");
    cJSON *open = cJSON_GetObjectItem(symbol_obj, "o");
    cJSON *timestamp = cJSON_GetObjectItem(symbol_obj, "t");
    cJSON *bar_volume = cJSON_GetObjectItem(symbol_obj, "v");
    cJSON *volume_weighted_average_price = cJSON_GetObjectItem(symbol_obj, "vw");
    
    if (close) bars.close = close->valuedouble;
    if (high) bars.high = high->valuedouble;
    if (low) bars.low = low->valuedouble;
    if (trade_count) bars.trade_count = trade_count->valueint;
    if (open) bars.open = open->valuedouble;
    if (bar_volume) bars.bar_volume = bar_volume->valueint;
    if (volume_weighted_average_price) bars.volume_weighted_average_price = volume_weighted_average_price->valuedouble;
    if (timestamp) {
        strncpy(bars.t, timestamp->valuestring, sizeof(bars.t) - 1);
        bars.t[sizeof(bars.t) - 1] = '\0';
    }
    
    cJSON_Delete(root);
    return bars;
}