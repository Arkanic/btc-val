#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <curl/curl.h>

#include "val.h"

struct {
    char *ticker;
    char *filename;
    int recenttx;
    short showrtx;
    short showval;
} Config;

void initconfig(void) {
    Config.ticker = "USD";
    Config.filename = "!!!";
    Config.recenttx = 5;
    Config.showrtx = 1;
    Config.showval = 1;
}

void showhelp(void) {
    printf("Usage: ./val [address to wallet]\nFile is a newline-separated list of wallet addresses\n");
    printf("Flags:\n"
            "--help                Show this help message\n"
            "--ticker [currency]   Set what fiat value the currency should be displayed in\n"
            "--recenttx [amount]   Number of recent transactions to show. Will be limited to the number actually available.\n"
            "--nortx               Do not show recent transactions\n"
            "--noval               Do not show wallet value\n"
    );
}

int main(int argc, char *argv[]) {
    initconfig();

    setlocale(LC_NUMERIC, "");

    if(argc == 1) {
        showhelp();
        return 1;
    } else if(argc > 1) {
        for(int j = 1; j < argc; j++) {
            int more = j+1 < argc;

            if(!strcmp(argv[j], "--ticker") && more) {
                Config.ticker = argv[++j];
            } else if(!strcmp(argv[j], "--help")) {
                showhelp();
                return 0;
            } else if(!strcmp(argv[j], "--recenttx") && more) {
                Config.recenttx = atoi(argv[++j]);
            } else if(!strcmp(argv[j], "--nortx")) {
                Config.showrtx = 0;
            } else if(!strcmp(argv[j], "--noval")) {
                Config.showval = 0;
            } else {
                Config.filename = argv[j];
            }
        }
    }

    if(strcmp(Config.filename, "!!!") == 0) {
        printf("No file chosen!\n");
        showhelp();
        return 1;
    }
    FILE *fp = fopen(Config.filename, "r");
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

    char *walletstr = api_getwalletstr(wallets, walletCount);
    unsigned long long value = api_walletsvalue(walletstr, walletCount);

    double price = api_btcprice(Config.ticker);
    
    if(Config.showval) {
        double valueBTC = value / 100000000.0f;
        double valueUSD = valueBTC * price;
        printf("BTC: %.6f BTC\nValue: $%'.2f %s\n%d Wallets\n\n", valueBTC, valueUSD, Config.ticker, walletCount);
    }

    if(Config.showrtx) {
        int transactionslength = 0;
        struct txn **transactions = api_recenttxns(&transactionslength, walletstr, Config.recenttx);
        printf("Recent Transactions:\n");
        for(int i = 0; i < transactionslength; i++) {
            struct txn *transaction = transactions[i];

            double diff = transaction->diff / 100000000.0f;
            char *when = timeago(transaction->time);

            char *color;
            if(diff < 0) {
                color = "\x1b[38;5;124m";
            } else {
                color = "\x1b[38;5;118m ";
            }

            printf("%s%.6fBTC\x1b[0m %s\n", color, diff, when);
            free(when);
        }
    }

    free(walletstr);

    api_shutdown();

    for(int i = 0; i < walletCount; i++) {
        free(wallets[i]);
    }
    free(wallets);

    return 0;
}