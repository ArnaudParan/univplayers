#include "gdbus_wrapping.h"

#include <stddef.h>

GError *__gio_error = NULL;

GVariant *send_media_message(const char *name, const char *message) {
    GDBusProxy *proxy = g_dbus_proxy_new_for_bus_sync(
            G_BUS_TYPE_SESSION,
            G_DBUS_PROXY_FLAGS_NONE,
            NULL,
            name,
            "/org/mpris/MediaPlayer2",
            "org.mpris.MediaPlayer2.Player",
            NULL,
            &__gio_error);
    return g_dbus_proxy_call_sync(proxy,
                                 message,
                                 g_variant_new ("()"),
                                 G_DBUS_CALL_FLAGS_NONE,
                                 -1,
                                 NULL,
                                 &__gio_error);
}

void send_media_message_to_all(const struct CStList *names, const char *message) {
    //TODO handle responses
    for (; names; names = names->next)
        send_media_message(names->val, message);
}

static GVariant *__get_players_glist() {
    GDBusProxy *proxy = g_dbus_proxy_new_for_bus_sync(
            G_BUS_TYPE_SESSION,
            G_DBUS_PROXY_FLAGS_NONE,
            NULL,
            "org.freedesktop.DBus",
            "/org/freedesktop/DBus",
            "org.freedesktop.DBus",
            NULL,
            &__gio_error);
    GVariant *names_tuple = g_dbus_proxy_call_sync(proxy, "ListNames", NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &__gio_error);
    GVariant *names_list = NULL;
    GVariantIter iter_tuple;

    if (!g_variant_is_container(names_tuple)) {
        return NULL;
    }
    g_variant_iter_init(&iter_tuple, names_tuple);
    names_list = g_variant_iter_next_value(&iter_tuple);

    if (!g_variant_is_container(names_list)) {
        return NULL;
    }

    return names_list;
}

struct CStList *list_available_players() {
    GVariant *names_list = __get_players_glist();
    GVariant *name_variant;
    GVariantIter iter_names;
    const char *name = NULL;
    struct CStList *ret = NULL;

    g_variant_iter_init(&iter_names, names_list);
    while ((name_variant = g_variant_iter_next_value(&iter_names))) {
        name = g_variant_get_string(name_variant, NULL);
        if (strncmp("org.mpris.MediaPlayer2.", name, 23) == 0) {
            CStList_push(&ret, name);
        }
    }

    CStList_rev(&ret);

    return ret;
}

