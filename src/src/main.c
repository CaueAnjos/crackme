#include "cmd.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

int register_hability(lua_State *L) {
  const char *hability_name = luaL_checkstring(L, 1);
  printf("[lua]: hability %s registered successfuly", hability_name);
  return 1;
}

int main(int argc, char **argv) {
  if (parse_args(argc, argv) != PARSE_SCRIPT) {
    return 1;
  }

  const char *script_path = argv[2];

  lua_State *L = luaL_newstate();
  if (!L) {
    fprintf(stderr, "Failed to create Lua state\n");
  }

  luaL_openlibs(L);
  lua_register(L, "register_hability", register_hability);

  FILE *script = fopen(script_path, "r");
  if (script == NULL) {
    fprintf(stderr, "Failed to open %s\n", script_path);
    return 1;
  }

  char line[1024];
  while (fgets(line, sizeof(line), script)) {
    luaL_dostring(L, line);
  }

  fclose(script);
  lua_close(L);

  return 0;
}
