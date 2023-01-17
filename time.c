#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "val.h"

char *periods[] = {"second", "minute", "hour", "day", "week", "month", "year", "decade"};
float lengths[] = {60, 60, 24, 7, 4.35, 12, 10};
int amount = 7;

char *timeago(unsigned long long time) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    unsigned long long diff = tv.tv_sec - time;

    int i;
    for(i = 0; diff >= lengths[i] && i < amount; i++) {
        diff /= lengths[i];
    }

    char *str = malloc(sizeof(char) * (strlen(periods[i]) + 1 + 1 + 1 + 4));
    sprintf(str, "%llu %s", diff, periods[i]);
    if(diff == 1) strcat(str, " ago");
    else strcat(str, "s ago");

    return str;
}