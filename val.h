#ifndef __VAL_H
#define __VAL_H

#include <stdlib.h>
#include <curl/curl.h>

struct string {
    char *ptr;
    size_t len;
};

void webget(CURL *curl, struct string *s, char *url);

#endif