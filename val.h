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
char *api_getwalletstr(char *wallets[], int walletcount);
unsigned long long api_walletsvalue(char *walletstr, int walletcount);
double api_btcprice(char *ticker);

struct txn {
    long long diff;
    unsigned long long time;
};

struct txn **api_recenttxns(int *resultCount, char *walletstr, int count);

#endif