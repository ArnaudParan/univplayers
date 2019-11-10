#include "argument_parsing.h"
#include <stddef.h>

void init_arguments(struct arguments* args) {
    args->silent = 0;
    args->verbose = 0;
    args->list_all = 0;
    args->follow = 0;
    args->all_players = 0;
    args->player = NULL;
    args->ignore_player = NULL;
    args->format = NULL;
    args->command = Undefined;
}
