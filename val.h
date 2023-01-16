#ifndef __VAL_H
#define __VAL_H

#include <stdlib.h>
#include <curl/curl.h>

// NET.C
struct string {
    char *ptr;
    size_t len;
};

void webget(CURL *curl, struct string *s, char *url);


// API.C
void api_init(void);
void api_shutdown(void);
float api_totalvalue(char *wallets[], int walletcount);

#endif