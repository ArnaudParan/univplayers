#include "storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined __linux__ || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__\
    || defined __bsdi__ || defined __DragonFly__ || __FreeBSD_kernel__ || __APPLE__
static const char *__default_storage_path = "/tmp/univplayers";
#endif

char *get_default_player() {
    FILE *file = fopen(__default_storage_path, "r");
    char *buf = calloc(255, sizeof(char));;
    if (file == NULL)
        return NULL;
    fgets(buf, 255, file);
    return buf;
}

void set_default_player(const char *player) {
    FILE *file = NULL;

    if (strpbrk(player, "\n\r "))
        // poorly formatted player, we won't store that
        return;

    file = fopen(__default_storage_path, "w");
    fputs(player, file);
}
