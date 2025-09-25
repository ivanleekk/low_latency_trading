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
        char env_entry[512];
        // Save the key-value pair in the format KEY=VALUE
        snprintf(env_entry, sizeof(env_entry), "%s=%s", key, value);
        // Place in env
        putenv(env_entry);
    }
    fclose(file);
}

int main(void) {
    load_env(".env");
    const char* api_key = getenv("API_KEY");
    if (api_key) {
        printf("API_KEY: %s\n", api_key);
    } else {
        printf("API_KEY not set\n");
    }
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.ivanleekaikiat.com");
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    return 0;
}