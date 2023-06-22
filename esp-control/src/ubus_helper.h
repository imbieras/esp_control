#ifndef UBUS_HELPER_H
#define UBUS_HELPER_H

#include <libubox/blob.h>
#include <libubox/blobmsg.h>
#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include <stdint.h>

enum {
  CONTROL_PORT,
  CONTROL_PIN,
  __CONTROL_MAX,
};

int ubus_init(struct ubus_context **ctx);
int ubus_deinit(struct ubus_context *ctx);
int devices(struct ubus_context *ctx, struct ubus_object *obj,
            struct ubus_request_data *req, const char *method,
            struct blob_attr *msg);
int on(struct ubus_context *ctx, struct ubus_object *obj,
       struct ubus_request_data *req, const char *method,
       struct blob_attr *msg);
int off(struct ubus_context *ctx, struct ubus_object *obj,
        struct ubus_request_data *req, const char *method,
        struct blob_attr *msg);

static const struct blobmsg_policy control_policy[] = {
    [CONTROL_PORT] = {.name = "port", .type = BLOBMSG_TYPE_STRING},
    [CONTROL_PIN] = {.name = "pin", .type = BLOBMSG_TYPE_INT32}};

static const struct ubus_method control_methods[] = {
    UBUS_METHOD_NOARG("devices", devices),
    UBUS_METHOD("on", on, control_policy),
    UBUS_METHOD("off", off, control_policy)};

static struct ubus_object_type control_object_type =
    UBUS_OBJECT_TYPE("esp_control", control_methods);

#endif // UBUS_HELPER_H
