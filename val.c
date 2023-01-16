#include <stdio.h>
#include <curl/curl.h>

#include "val.h"

int main(void) {
    api_init();

    char *wallets[] = {"34xp4vRoCGJym3xR7yCVPFHoCNxv4Twseo", "bc1qgdjqv0av3q56jvd82tkdjpy7gdp9ut8tlqmgrpmv24sq90ecnvqqjwvw97"};
    unsigned long long value = api_totalvalue(wallets, 2);
    double price = api_btcprice();
    
    double valueBTC = value / 100000000.0f;
    double valueUSD = valueBTC * price;
    printf("Sats: %llusat\nBTC: %fBTC\nPrice: $%f\n", value, valueBTC, valueUSD);

    api_shutdown();
    return 0;
}