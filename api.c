#include <json-c/json.h>
#include <json-c/json_object.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

#include "val.h"

CURL *curl;
struct string s;

void api_init(void) {
    curl = curl_easy_init();
}

void api_shutdown(void) {
    curl_easy_cleanup(curl);
}

char *api_getwalletstr(char *wallets[], int walletcount) {
    char *baseurl = "blockchain.info/multiaddr?active=";
    int len = strlen(baseurl);
    len += 63 * walletcount; // extra is for pipe, 62 is for segwit address (longest)

    char *url = malloc((sizeof(char) * len) + 1);
    strcpy(url, baseurl);
    for(int i = 0; i < walletcount; i++) {
        strcat(url, wallets[i]);
        strcat(url, "|");
    }

    webget(curl, &s, url);

    return s.ptr; //  needs to be freed
}

unsigned long long api_walletsvalue(char *walletstr, int walletcount) {
    struct json_object *response = json_tokener_parse(s.ptr);
    struct json_object *responseWallet = json_object_object_get(response, "wallet");
    struct json_object *responseFinalBalance = json_object_object_get(responseWallet, "final_balance");
    unsigned long long finalBalance = json_object_get_int64(responseFinalBalance);

    json_object_put(response);

    return finalBalance;
}

// returns length of out array
struct txn **api_recenttxns(int *resultCount, char *walletstr, int count) {
    struct json_object *wallets = json_tokener_parse(walletstr);
    struct json_object *txs = json_object_object_get(wallets, "txs");

    int txslen = json_object_array_length(txs);
    if(txslen < count) count = txslen;

    struct txn **out = (struct txn **)malloc(sizeof(struct txn *) * count);
    for(int i = 0; i < count; i++) {
        struct txn *transaction = (struct txn *)malloc(sizeof(struct txn));
        out[i] = transaction;
        
        struct json_object *jsontxn = json_object_array_get_idx(txs, i);
        
        long long result = json_object_get_int64(json_object_object_get(jsontxn, "result"));
        transaction->diff = result;

        unsigned long long time = json_object_get_int64(json_object_object_get(jsontxn, "time"));
        transaction->time = time;
    }

    json_object_put(wallets);

    *resultCount = count;
    return out;
}

void api_recenttxns_freeout(struct txn *out[], int length) {
    for(int i = 0; i < length; i++) {
        free(out[i]);
    }
    free(out);
}

double api_btcprice(char *ticker) {
    char *url = "blockchain.info/ticker";
    webget(curl, &s, url);

    struct json_object *response = json_tokener_parse(s.ptr);
    struct json_object *responseUSD = json_object_object_get(response, ticker);
    struct json_object *responseUSD15m = json_object_object_get(responseUSD, "15m");
    double priceUSD = json_object_get_double(responseUSD15m);

    free(s.ptr);

    return priceUSD;
}