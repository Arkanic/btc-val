#include <stdio.h>
#include <curl/curl.h>

#include "val.h"

int main(void) {
    struct string s;
    CURL *curl = curl_easy_init();

    webget(curl, &s, "arkanic.github.io");
    printf("%s\n", s.ptr);

    free(s.ptr);
    curl_easy_cleanup(curl);
}