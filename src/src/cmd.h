#ifndef CMD_H
#define CMD_H

#include <stdint.h>

#define PARSE_UNDEFINED 0
#define PARSE_EMPTY 1
#define PARSE_SCRIPT 2
#define PARSE_HELP 3

uint32_t parse_args(int32_t argc, char **argv);
void print_options(int32_t argc, char **argv);

#endif
