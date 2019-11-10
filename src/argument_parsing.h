#ifndef ARGUMENT_PARSING_H
#define ARGUMENT_PARSING_H

#define _HASH_MAX // TODO
#include <argp.h>
#include <string.h>

static char doc[] = "Argp example -- a simple program";
static char args_doc[] = "COMMAND";

static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "produce verbose output", 0},
    {"quiet", 'q', 0, 0, "Don't produce any output", 0},
    {"player", 'p', "NAME", 0, "The name or comma-separated list of the players to control (default: first available player)", 0},
    {"ignore-player", 'i', "NAME", 0, "The name or comma-separated list of the players to ignore", 0},
    {"format", 'f', "FORMAT", 0, "A format string for printing properties and metadata", 0},
    {"follow", 'F', 0, 0, "Block and output the updated query when it changes", 0},
    {"list-all", 'l', 0, 0, "List the names of running players that can be controlled", 0},
    {"all-players", 'a', 0, 0, "Apply command to all available players", 0},
    {0, 0, 0, 0, 0, 0}
};

enum Command {
    Status,
    Play,
    Pause,
    PlayPause,
    Stop,
    Next,
    Previous,
    Position,
    Volume,
    Metadata,
    Open,
    Shuffle,
    Loop,
    ShufflePlayer,
    Undefined
};

struct arguments {
    enum Command command;
    char silent, verbose, list_all, follow, all_players;
    char *player, *ignore_player, *format;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = (struct arguments*)state->input;

    switch (key) {
        case 'q': case 's':
            arguments->silent = 1;
            break;
        case 'v':
            arguments->verbose = 1;
            break;
        case 'l':
            arguments->list_all = 1;
            break;
        case 'p':
            arguments->player = arg;
            break;
        case 'i':
            arguments->ignore_player = arg;
            break;
        case 'f':
            arguments->format = arg;
            break;
        case 'a':
            arguments->all_players = 1;
            break;
        case 'F':
            arguments->follow = 1;
            break;
        case ARGP_KEY_ARG:
            if (arguments->command == Status || arguments->command == Play || arguments->command == Pause ||
                    arguments->command == PlayPause || arguments->command == Stop || arguments->command == Next ||
                    arguments->command == Previous)
                argp_error(state, "You provided too many arguments");
            if (strcmp("status", arg) == 0)
                arguments->command = Status;
            if (strcmp("play", arg) == 0)
                arguments->command = Play;
            if (strcmp("pause", arg) == 0)
                arguments->command = Pause;
            if (strcmp("play-pause", arg) == 0)
                arguments->command = PlayPause;
            if (strcmp("stop", arg) == 0)
                arguments->command = Stop;
            if (strcmp("next", arg) == 0)
                arguments->command = Next;
            if (strcmp("previous", arg) == 0)
                arguments->command = Previous;
            if (strcmp("position", arg) == 0)
                arguments->command = Position;
            if (strcmp("volume", arg) == 0)
                arguments->command = Volume;
            if (strcmp("metadata", arg) == 0)
                arguments->command = Metadata;
            if (strcmp("open", arg) == 0)
                arguments->command = Open;
            if (strcmp("shuffle", arg) == 0)
                arguments->command = Shuffle;
            if (strcmp("loop", arg) == 0)
                arguments->command = Loop;
            if (strcmp("shuffle-player", arg) == 0)
                arguments->command = ShufflePlayer;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

__attribute__((unused))
static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

void init_arguments(struct arguments* args);

#endif
