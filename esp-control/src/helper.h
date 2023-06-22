#ifndef HELPER_H
#define HELPER_H

#include <libubox/blobmsg_json.h>
#include <limits.h>
#include <stdio.h>

#define BUFFER_SIZE 1024
#define DEVICE_MAX 8
#define ESP_PID 0xEA60
#define ESP_VID 0x10C4

struct device {
  char port[16];
  char pid[9];
  char vid[9];
};

int open_port(char *port);
int serial_in(int fd, char *message);
int serial_out(int fd, char *message);
int send_to_esp(char *port, char *message, struct blob_buf *b);
int list_devices(struct device *devices);
int device_blob_buf(struct device *devices, int count, struct blob_buf *b);

#endif // HELPER_H
