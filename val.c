#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "val.h"

int main(int argc, char *argv[]) {
    char *filename = "./wallets.txt";
    if(argc > 1) filename = argv[1];

    FILE *fp = fopen(filename, "r");
    if(fp == NULL) return 1;

    char **wallets = (char **)malloc(sizeof(char *));
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int i = 0;
    while((read = getline(&line, &len, fp)) != -1) {

        wallets[i] = (char *)malloc(sizeof(char) * (read + 1));
        strcpy(wallets[i], line);
        char end = wallets[i][strlen(line) - 1];
        if(end == '\n') wallets[i][strlen(line) - 1] = '\0';

        i++;
        wallets = realloc(wallets, sizeof(char *) * (i + 1));
    }

    api_init();

    unsigned long long value = api_totalvalue(wallets, i);
    double price = api_btcprice();
    
    double valueBTC = value / 100000000.0f;
    double valueUSD = valueBTC * price;
    printf("Sats: %llusat\nBTC: %fBTC\nPrice: $%f\n", value, valueBTC, valueUSD);

    api_shutdown();

    for(int j = 0; j < i; j++) {
        free(wallets[j]);
    }
    free(wallets);

    return 0;
}