#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

int fd;
struct termios *configure;

int8_t tty_config(struct termios *con, int descriptor);
char read_one_byte(int fd);
int8_t read_bytes(int fd, char *str);
int8_t transfer_one_byte(int fd);
int8_t transfer_bytes(int fd, char *str);


#endif