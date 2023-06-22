#include "ubus_helper.h"
#include "helper.h"
#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int ubus_init(struct ubus_context **ctx) {
  uloop_init();
  *ctx = ubus_connect(NULL);
  if (!(*ctx)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int ubus_deinit(struct ubus_context *ctx) {
  if (!ctx) {
    return EXIT_FAILURE;
  }
  ubus_free(ctx);
  uloop_done();
  return EXIT_SUCCESS;
}

int devices(struct ubus_context *ctx, struct ubus_object *obj,
            struct ubus_request_data *req, const char *method,
            struct blob_attr *msg) {
  struct blob_buf b = {};
  blob_buf_init(&b, 0);

  struct device devices[DEVICE_MAX];
  int count = list_devices(devices);
  if (count <= 0)
    return UBUS_STATUS_NO_DATA;
  device_blob_buf(devices, count, &b);
  ubus_send_reply(ctx, req, b.head);
  blob_buf_free(&b);

  return EXIT_SUCCESS;
}

int on(struct ubus_context *ctx, struct ubus_object *obj,
       struct ubus_request_data *req, const char *method,
       struct blob_attr *msg) {
  struct blob_attr *tb[__CONTROL_MAX];
  struct blob_buf b = {};
  blob_buf_init(&b, 0);
  blobmsg_parse(control_policy, __CONTROL_MAX, tb, blob_data(msg),
                blob_len(msg));

  if (!tb[CONTROL_PORT] || !tb[CONTROL_PIN])
    return UBUS_STATUS_INVALID_ARGUMENT;

  char *port = blobmsg_get_string(tb[CONTROL_PORT]);
  int pin = blobmsg_get_u32(tb[CONTROL_PIN]);

  char message[BUFFER_SIZE];
  sprintf(message, "{\"action\": \"on\", \"pin\": %d}", pin);

  if ((send_to_esp(port, message, &b)) != EXIT_SUCCESS)
    return UBUS_STATUS_INVALID_ARGUMENT;
  ubus_send_reply(ctx, req, b.head);
  blob_buf_free(&b);

  return EXIT_SUCCESS;
}

int off(struct ubus_context *ctx, struct ubus_object *obj,
        struct ubus_request_data *req, const char *method,
        struct blob_attr *msg) {
  struct blob_attr *tb[__CONTROL_MAX];
  struct blob_buf b = {};
  blob_buf_init(&b, 0);
  blobmsg_parse(control_policy, __CONTROL_MAX, tb, blob_data(msg),
                blob_len(msg));

  if (!tb[CONTROL_PORT] || !tb[CONTROL_PIN])
    return UBUS_STATUS_INVALID_ARGUMENT;

  char *port = blobmsg_get_string(tb[CONTROL_PORT]);
  int pin = blobmsg_get_u32(tb[CONTROL_PIN]);

  char message[BUFFER_SIZE];
  sprintf(message, "{\"action\": \"off\", \"pin\": %d}", pin);

  if ((send_to_esp(port, message, &b)) != EXIT_SUCCESS)
    return UBUS_STATUS_INVALID_ARGUMENT;
  ubus_send_reply(ctx, req, b.head);
  blob_buf_free(&b);

  return EXIT_SUCCESS;
}
