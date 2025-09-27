#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <curl/curl.h>
#include "http.h"
#include "alpaca.h"

// Function to trim whitespace from start and end
char* trim(char* str) {
    char* end;
    // Trim leading spaces
    while (isspace((unsigned char)*str)) str++;
    // If null pointer return as is
    if (*str == 0) return str;
    // find pointer for end of string
    end = str + strlen(str) - 1;
    // Trim trailing spaces
    while (end > str && isspace((unsigned char)*end)) end--;
    // Set new null terminator
    end[1] = '\0';
    return str;
}

void load_env(const char* filename) {
  // Open the .env file as read-only
    FILE* file = fopen(filename, "r");
    // if file pointer is null, print error and return
    if (!file) {
        fprintf(stderr, "Could not open %s\n", filename);
        return;
    }
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char* trimmed = trim(line);
        // ignore if commented out or is null
        if (trimmed[0] == '#' || trimmed[0] == '\0') continue;
        char* eq = strchr(trimmed, '=');
        // if no equal sign is found, continue to next line
        if (!eq) continue;
        *eq = '\0';
        // replace = with null terminator, now trim to get key and value
        char* key = trim(trimmed);
        char* value = trim(eq + 1);
        // Allocate memory for the environment string
        size_t len = strlen(key) + strlen(value) + 2; // '=' and '\0'
        char* env_entry = (char*)malloc(len);
        if (!env_entry) {
            fprintf(stderr, "Memory allocation failed for env_entry\n");
            continue;
        }
        snprintf(env_entry, len, "%s=%s", key, value);
        putenv(env_entry); // Do not free env_entry!
    }
    fclose(file);
}

int main(void) {
    load_env(".env");
    CURL *curl;
    CURLcode res;
    char response[8192] = {0};


    // curl = httpget("https://data.alpaca.markets/v2/stocks/bars/latest?symbols=AAPL", headers);
    alpaca_get_account(response, sizeof(response));

    printf("Response: %s\n", response);
    alpaca_get_latest_bars("AAPL", response, sizeof(response));
    printf("Response: %s\n", response);
    alpaca_post_order("{\"symbol\":\"AAPL\",\"qty\":1,\"side\":\"buy\",\"type\":\"market\",\"time_in_force\":\"day\"}", response, sizeof(response));
    printf("Response: %s\n", response);
    return 0;
}