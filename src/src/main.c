#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

int register_hability(lua_State *L) {
  int a = luaL_checkinteger(L, 1);
  int b = luaL_checkinteger(L, 2);
  lua_pushinteger(L, a + b);
  return 1;
}

int main(int argc, char **argv) {
  printf("%d:\n ", argc);
  for (int i; i < 2; i++) {
    printf("\t-%s", argv[i]);
  }
  return 0;

  lua_State *L = luaL_newstate();
  if (!L) {
    fprintf(stderr, "Failed to create Lua state\n");
  }

  luaL_openlibs(L);
  lua_register(L, "register_hability", register_hability);

  char line[1024] = "";
  if (fgets(line, sizeof(line), stdin)) {
    luaL_dostring(L, line);
  }

  lua_close(L);

  return 0;
}
