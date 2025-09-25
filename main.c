#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <curl/curl.h>

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

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://paper-api.alpaca.markets/v2/account");
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        struct curl_slist *headers = NULL;
        char* api_key = getenv("ALPACA_API_KEY");
        char* api_secret = getenv("ALPACA_API_SECRET_KEY");
        printf("API Key: %s\n", api_key);
        printf("API Secret: %s\n", api_secret);
        char header_key[128];
        char header_secret[128];
        snprintf(header_key, sizeof(header_key), "APCA-API-KEY-ID: %s", api_key);
        snprintf(header_secret, sizeof(header_secret), "APCA-API-SECRET-KEY: %s", api_secret);
        printf("Header Key: %s\n", header_key);
        printf("Header Secret: %s\n", header_secret);
        headers = curl_slist_append(headers, header_key);
        headers = curl_slist_append(headers, header_secret);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    return 0;
}