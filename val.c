#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "val.h"

int main(int argc, char *argv[]) {
    char *filename;
    if(argc == 1) {
        printf("Usage: ./val [address to wallet]\nFile is a newline-separated list of wallet addresses\n");
        return 1;
    } else if(argc > 1) filename = argv[1];

    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Selected file was not found or could not be opened.\n");
        return 1;
    }

    char **wallets = (char **)malloc(sizeof(char *));
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int walletCount = 0;
    while((read = getline(&line, &len, fp)) != -1) {

        wallets[walletCount] = (char *)malloc(sizeof(char) * (read + 1));
        strcpy(wallets[walletCount], line);
        char end = wallets[walletCount][strlen(line) - 1];
        if(end == '\n') wallets[walletCount][strlen(line) - 1] = '\0';

        walletCount++;
        wallets = realloc(wallets, sizeof(char *) * (walletCount + 1));
    }

    api_init();

    unsigned long long value = api_totalvalue(wallets, walletCount);
    double price = api_btcprice();
    
    double valueBTC = value / 100000000.0f;
    double valueUSD = valueBTC * price;
    printf("Sats: %llusat\nBTC: %fBTC\nPrice: $%f\n", value, valueBTC, valueUSD);

    api_shutdown();

    for(int i = 0; i < walletCount; i++) {
        free(wallets[i]);
    }
    free(wallets);

    return 0;
}