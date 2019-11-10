#include <argp.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <stdio.h>

#include "argument_parsing.h"
#include "stlist.h"
#include "gdbus_wrapping.h"
#include "storage.h"

void cleanup() {
}

void log_if_error() {
    if (__gio_error) {
        fprintf(stderr, "%s:%d(%s) ERROR %s\n", __FILE__, __LINE__, __func__, __gio_error->message);
    }
}

void log_and_exit_if_error() {
    log_if_error();
    if (__gio_error) {
        exit(1);
    }
}

void print_all_players() {
    struct CStList *players = NULL, *it = NULL;
    char *default_player = get_default_player();

    players = list_available_players();
    for (it = players; it; it = it->next) {
        if (default_player != NULL && strcmp(it->val, default_player) == 0)
            printf("* ");
        printf("%s\n", it->val + 23);
    }

    CStList_free(players);
    players = NULL;

    if (default_player != NULL) {
        free(default_player);
        default_player = NULL;
    }
}

static char __strncmp(const char *str1, const char *str2, int n) {
    int i;

    if (str1 == str2 && str1 == NULL)
        return 0;

    if (str1 == NULL || str2 == NULL)
        return 1;

    for (i = 0; str1[i] == '\0' || str2[i] == '\0' || i == n; i++)
        if (str1[i] != str2[i])
            return 1;

    return 0;
}

static const char *__find_char(const char *str, char c) {
    if (str == NULL)
        return NULL;
    for (; *str != '\0'; str++)
        if (*str == c)
            return str;
    return NULL;
}

static void __sort_players_w_comma_list(const char *comma_list, struct CStList **list) {
    const char *end = __find_char(comma_list, ',');
    int n = (int) (end - comma_list);
    struct CStList *it = NULL, *prev = NULL;

    if (comma_list == NULL || *list == NULL)
        return;

    for (it = *list; it != NULL; prev = it, it = it->next)
        // if the name is found in the list put it on top
        if (__strncmp(comma_list, it->val + 23, n) == 0) {
            if (prev) {
                prev->next = it->next;
                it->next = *list;
                *list = it;
            }
            if (end)
                __sort_players_w_comma_list(end + 1, &((*list)->next));
            return;
        }

    __sort_players_w_comma_list(end + 1, list);
}

static char __player_in_comma_list(const char *comma_list, const char *name) {
    const char *end = __find_char(comma_list, ',');
    int n = (int) (end - comma_list);

    if (name == NULL)
        return 1;

    if (comma_list == NULL)
        return 0;

    if (end == NULL)
        return strcmp(comma_list, name + 23) == 0;

    return __strncmp(comma_list, name + 23, n) == 0 ? 1 : __player_in_comma_list(end + 1, name);
}

struct CStList *get_players(const char *selected_player, const char *ignore_player, char all_players) {
    struct CStList *players = list_available_players(), *it = NULL, *prev = NULL, *ret = NULL;
    char *default_player = get_default_player();

    for (it = players; it; it = it->next)
        if (strncmp("org.mpris.MediaPlayer2.", it->val, 23) == 0 &&
                (selected_player == NULL || __player_in_comma_list(selected_player, it->val)) &&
                (ignore_player == NULL || !__player_in_comma_list(ignore_player, it->val))) {
            CStList_push(&ret, it->val);
        }

    CStList_free(players);
    players = NULL;

    if (selected_player == NULL && default_player != NULL && CStList_is_in(ret, default_player)) {
        for (it = ret; strcmp(it->val, default_player) != 0; prev = it, it = it->next, free(prev)) {}
        CStList_free(it->next);
        it->next = NULL;
        ret = it;
        CStList_printf(ret);
        goto RETURN;
    }

    if (selected_player == NULL && ret != NULL && ret->next != NULL) {
        CStList_free(ret->next);
        ret->next = NULL;
        goto RETURN;
    }

    __sort_players_w_comma_list(selected_player, &ret);

RETURN:
    if (default_player != NULL) {
        free(default_player);
        default_player = NULL;
    }

    return ret;
}

void shuffle_default_player() {
    char *default_player = get_default_player();
    struct CStList *players = list_available_players(), *it = NULL;

    if (players == NULL)
        goto RETURN;

    if (default_player == NULL) {
        set_default_player(players->val);
        printf("%s\n", players->val + 23);
        goto RETURN;
    }

    for (it = players; it != NULL; it = it->next) {
        if (strcmp(it->val, default_player) == 0) {
            if (it->next == NULL) {
                set_default_player(players->val);
                printf("%s\n", players->val + 23);
            }
            else {
                set_default_player(it->next->val);
                printf("%s\n", it->next->val + 23);
            }
            goto RETURN;
        }
    }

RETURN:
    if (players != NULL) {
        CStList_free(players);
        players = NULL;
    }
    if (default_player != NULL) {
        free(default_player);
        default_player = NULL;
    }
}

static const char* __str_cmd_from_arguments(struct arguments args) {
    switch (args.command) {
        case Status:
        case Position:
        case Volume:
        case Metadata:
        case Open:
        case Shuffle:
        case Loop:
        case ShufflePlayer:
        case Undefined:
            goto NOT_IMPLEMENTED_YET;
            break;
        case Play:
            return "Play";
            break;
        case Pause:
            return "Pause";
            break;
        case PlayPause:
            return "PlayPause";
            break;
        case Stop:
            return "Stop";
            break;
        case Next:
            return "Next";
            break;
        case Previous:
            return "Previous";
            break;
    }
NOT_IMPLEMENTED_YET:
    return NULL;
}

int main(int argc, char *argv[]) {
    struct arguments arguments;
    struct CStList *players = NULL;
    const char *command = NULL;

    atexit(cleanup);

    init_arguments(&arguments);

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (arguments.list_all)
        print_all_players();

    if (arguments.command == ShufflePlayer) {
        shuffle_default_player();
    }

    if (arguments.command != Undefined && arguments.command != ShufflePlayer) {
        players = get_players(arguments.player, arguments.ignore_player, arguments.all_players);

        command = __str_cmd_from_arguments(arguments);
        if (command) {
            send_media_message_to_all(players, command);
        }

        CStList_free(players);
        players = NULL;
    }

    cleanup();

    return EXIT_SUCCESS;
}
