#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "val.h"

void initstr(struct string *s) {
    s->len = 0;
	s->ptr = malloc(s->len + 1);
	if(s->ptr == NULL) {
		fprintf(stderr, "malloc() failed\n");
		exit(1);
	}
	s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
	size_t new_len = s->len + size * nmemb;
	s->ptr = realloc(s->ptr, new_len + 1);
	if(s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(1);
	}
	memcpy(s->ptr+s->len, ptr, size * nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return size * nmemb;
}

void webget(CURL *curl, struct string *s, char *url) {
    initstr(s);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, s);

    CURLcode res = curl_easy_perform(curl);
}