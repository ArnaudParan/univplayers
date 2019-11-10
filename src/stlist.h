#ifndef STLIST_H
#define STLIST_H
#include <stdio.h>

struct StList {
    struct StList *next;
    char *val;
};

struct CStList {
    struct CStList *next;
    const char *val;
};

void CStList_push(struct CStList **list, const char *val);
void CStList_free(struct CStList *list);
void CStList_rev(struct CStList **list);
void CStList_fprintf(FILE *stream, const struct CStList *list);
void CStList_printf(const struct CStList *list);
char CStList_is_in(const struct CStList *list, const char *elem);

void StList_push(struct StList **list, char *val);
void StList_free_val(struct StList *list);
void StList_free(struct StList *list);
void StList_rev(struct StList **list);
void StList_fprintf(FILE *stream, const struct StList *list);
void StList_printf(const struct StList *list);

#endif
