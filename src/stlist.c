#include "stlist.h"
#include <stdlib.h>
#include <string.h>

void CStList_push(struct CStList **list, const char *val) {
    struct CStList *elem = malloc(sizeof(struct CStList));
    elem->next = *list;
    elem->val = val;
    *list = elem;
}

void CStList_free(struct CStList *list) {
    if (list == NULL) {
        return;
    }
    CStList_free(list->next);
    free(list);
}

static struct CStList *__slr_rev(struct CStList *list) {
    struct CStList *ret;
    if (list == NULL)
        return NULL;
    if (list->next == NULL)
        return list;
    ret = __slr_rev(list->next);
    list->next->next = list;
    list->next = NULL;
    return ret;
}

void CStList_rev(struct CStList **list) {
    *list = __slr_rev(*list);
}

void CStList_fprintf(FILE *stream, const struct CStList *list) {
    //TODO detect when too long
    fprintf(stream, "[ ");
    for (; list; list = list->next)
        fprintf(stream, "\"%s\" ", list->val);
    fprintf(stream, "]");
}

void CStList_printf(const struct CStList *list) {
    CStList_fprintf(stdout, list);
}

char CStList_is_in(const struct CStList *list, const char *elem) {
    if (list == NULL)
        return 0;
    return strcmp(list->val, elem) == 0 ? 1 : CStList_is_in(list->next, elem);
}

void StList_push(struct StList **list, char *val) {
    CStList_push((struct CStList**) list, (const char*) val);
}


void StList_free_val(struct StList *list) {
    for (; list != NULL; list = list->next) {
        free(list->val);
    }
}

void StList_free(struct StList *list) {
    CStList_free((struct CStList*) list);
}

void StList_rev(struct StList **list) {
    CStList_rev((struct CStList**) list);
}

void StList_fprintf(FILE *stream, const struct StList *list) {
    CStList_fprintf(stream, (const struct CStList*) list);
}

void StList_printf(const struct StList *list) {
    CStList_printf((const struct CStList*) list);
}
