#include "cmd.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t parse_args(int32_t argc, char **argv) {
  for (size_t i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_options(argc, argv);
      return PARSE_HELP;
    }

    if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--script") == 0) {
      if (argv[++i] != NULL && strlen(argv[i]) != 0) {
        return PARSE_SCRIPT;
      } else {
        fprintf(stderr, "The script path is empty! please pass a valid path\n");
        return PARSE_UNDEFINED;
      }
    }

    fprintf(stderr, "What kind of option is that? I just understand these:\n");
    print_options(argc, argv);
    return PARSE_UNDEFINED;
  }

  print_options(argc, argv);
  return PARSE_EMPTY;
}

void print_options(int32_t argc, char **argv) {
  if (argc < 1) {
    return;
  }

  printf("%s [options]\n", argv[0]);
  puts("options:");
  puts("-s, --script\truns a lua script");
}
