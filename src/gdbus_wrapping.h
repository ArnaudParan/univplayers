#ifndef GDBUS_WRAPPING_H
#define GDBUS_WRAPPING_H

#include <gio/gio.h>

#include "stlist.h"

extern GError *__gio_error;

GVariant *send_media_message(const char *name, const char *message);
void send_media_message_to_all(const struct CStList *names, const char *message);
struct CStList *list_available_players();

#endif
