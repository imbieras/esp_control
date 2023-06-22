#include "helper.h"
#include <fcntl.h>
#include <libserialport.h>
#include <libubox/blobmsg.h>
#include <libubox/blobmsg_json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int open_port(char *port) {
  struct termios tty;

  int fd = open(port, O_RDWR | O_NDELAY | O_NOCTTY | O_SYNC);

  if (tcgetattr(fd, &tty) < 0) {
    return -1;
  }

  tty.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
  tty.c_iflag = IGNPAR;
  tty.c_oflag = 0;
  tty.c_lflag = 0;

  tcflush(fd, TCIFLUSH);
  if (tcsetattr(fd, TCSANOW, &tty) != 0) {
    return -1;
  }
  return fd;
}

int serial_in(int fd, char *message) {
  sleep(2);
  memset(message, 0, BUFFER_SIZE);
  int bytes_read = read(fd, message, BUFFER_SIZE);
  return bytes_read;
}

int serial_out(int fd, char *message) {
  int bytes_written = write(fd, message, strlen(message));
  return bytes_written;
}

int send_to_esp(char *port, char *message, struct blob_buf *b) {
  int len;
  int fd = open_port(port);
  if (fd == -1) {
    return -1;
  }

  len = serial_out(fd, message);
  len = serial_in(fd, message);
  blobmsg_add_json_from_string(b, message);
  close(fd);
  return EXIT_SUCCESS;
}

int list_devices(struct device *devices) {
  struct sp_port **port_list;
  int count = 0;

  enum sp_return result = sp_list_ports(&port_list);

  if (result != SP_OK) {
    return -1;
  }

  for (int i = 0; port_list[i] != NULL; i++) {
    struct sp_port *port = port_list[i];

    char *port_name = sp_get_port_name(port);
    enum sp_transport transport = sp_get_port_transport(port);
    if (transport != SP_TRANSPORT_USB) {
      continue;
    }
    int usb_vid, usb_pid;
    sp_get_port_usb_vid_pid(port, &usb_vid, &usb_pid);
    if (usb_pid == ESP_PID && usb_vid == ESP_VID && count < DEVICE_MAX) {
      char pid[9];
      char vid[9];
      sprintf(pid, "%04X", usb_pid);
      sprintf(vid, "%04X", usb_vid);
      strncpy(devices[count].port, port_name, sizeof(devices[count].port) - 1);
      strncpy(devices[count].pid, pid, sizeof(devices[count].pid) - 1);
      strncpy(devices[count].vid, vid, sizeof(devices[count].vid) - 1);
      count++;
    }
  }
  sp_free_port_list(port_list);
  return count;
}

int device_blob_buf(struct device *devices, int count, struct blob_buf *b) {
  void *array = blobmsg_open_array(b, "devices");

  for (int i = 0; i < count; i++) {
    void *table = blobmsg_open_table(b, devices[i].port);
    blobmsg_add_string(b, "port", devices[i].port);
    blobmsg_add_string(b, "pid", devices[i].pid);
    blobmsg_add_string(b, "vid", devices[i].vid);
    blobmsg_close_table(b, table);
  }
  blobmsg_close_array(b, array);

  return EXIT_SUCCESS;
}
