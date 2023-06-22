#include "helper.h"
#include "ubus_helper.h"
#include <libubus.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

static struct ubus_object control_object = {.name = "esp_control",
                                            .type = &control_object_type,
                                            .methods = control_methods,
                                            .n_methods =
                                                ARRAY_SIZE(control_methods)};

int main(int argc, char **argv) {

  struct ubus_context *ctx = NULL;

  if (ubus_init(&ctx) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  ubus_add_uloop(ctx);
  ubus_add_object(ctx, &control_object);
  uloop_run();

  ubus_deinit(ctx);
  return EXIT_SUCCESS;
}
